#!/usr/bin/env python3
"""This is the build script for the cp project."""
# Copyright (c) 2025 user-name-beta. All rights reserved.
# Licensed under the MIT license.
# See LICENSE file in the project root for full license information.

import os
import sys
import shutil
import subprocess

# Helper functions

def report(s):
    """Report a message to the user through stderr."""
    print(f"build.py: {s}", file=sys.stderr)

def report_error(s):
    """Report an error message to the user through stderr and then exit."""
    print(f"build.py: error: {s}", file=sys.stderr)
    sys.exit(1)

def cmd(cmd, *args):
    """Call a command."""
    report(f"calling {cmd} {' '.join(args)}")
    if subprocess.call([cmd] + list(args)) != 0:
        report_error(f"command return code is 1")

# Read environ.env for environ variables

try:
    with open(".env", "r", encoding="utf-8") as env_file:
        report("using environ.env for environ variables")
        for line in env_file:
            line = line.strip()
            if line.startswith("#") or not line:
                continue
            key, value = line.split("=", 1)
            if key not in os.environ:
                os.environ[key] = value
except FileNotFoundError:
    pass

# Get version information from VERSION file

with open("VERSION", "r", encoding="utf-8") as version_file:
    for line in version_file:
        line = line.strip()
        if line.startswith("#"):
            continue
        if line:
            major, minor, micro = line.split(".")
            break
    else:
        report_error("VERSION file is invalid")

major_number = int(major)
minor_number = int(minor)
micro_number = int(micro)
version_hex = (major_number << 16) | (minor_number << 8) | micro_number

# Choose 64-bit or 32-bit compiler

if (gcc64_bin := os.environ.get("CPBUILDGCC64")) is None:
    report_error("CPBUILDGCC64 environment variable not set")
if (gcc32_bin := os.environ.get("CPBUILDGCC32")) is None:
    report_error("CPBUILDGCC32 environment variable not set")

argv = sys.argv[1:]
flags: list[str] = []
for i, flag in list(enumerate(argv)):
    if flag.startswith("-"):
        flags.append(flag)
        argv.pop(i)

if '--64' in flags:
    flags.remove('--64')
    gcc_bin = gcc64_bin
elif '--32' in flags:
    flags.remove('--32')
    gcc_bin = gcc32_bin
else:
    if (bit := os.environ.get("CPBUILDBIT")) is not None:
        if bit == "64":
            gcc_bin = gcc64_bin
        elif bit == "32":
            gcc_bin = gcc32_bin
        else:
            report_error("CPBUILDBIT environment variable is invalid")
    else:
        if sys.maxsize > 2**32:
            report("no architecture specified, assuming 64-bit as current device")
            gcc_bin = gcc64_bin
        else:
            report("no architecture specified, assuming 32-bit as current device")
            gcc_bin = gcc32_bin
del gcc64_bin, gcc32_bin

# Handle flags

if "--skip-exist" in flags:
    flags.remove("--skip-exist")
    skip_exist = True
elif "--no-skip-exist" in flags:
    flags.remove("--no-skip-exist")
    skip_exist = False
else:
    skip_exist = True

def gcc(cmd, *args):
    """Call a gcc command. 
    
    The program actually called depends on the environ variable 
    CPBUILDGCC64 or CPBUILDGCC32 and the flag --64 or --32 or the
    current architecture of the device.
    """
    report(f"calling {cmd} {' '.join(args)}")
    if subprocess.call([os.path.join(gcc_bin, cmd)] + list(args)) != 0:
        report_error(f"command return code is 1")

# Build classes

class Builder:
    def __init__(self, targets: list["Target"] | None = None):
        if targets is None:
            targets = []
        self.targets = targets
    
    def add_target(self, *target: "Target"):
        self.targets.extend(target)
    
    def build(self):
        report("building")
        # Create build directory
        if not os.path.exists("build"):
            report("build does not exist, creating")
            os.mkdir("build")
        if not os.path.isdir("build"):
            report_error("build is not a directory")
        if argv:
            targets = []
            for name in argv:
                for target in self.targets:
                    if target.name == name:
                        targets.append(target)
                        break
                else:
                    report_error(f"target {name} not found")
            for target in targets:
                report(f"building target {target.name}")
                target.build()
        else:
            for target in self.targets:
                if target.name == "all":
                    report(f"no target specified, building all")
                    target.build()
                    break
            else:
                report_error("no target specified but no target named `all` found")
        report("build complete successfully")

class Target:
    def __init__(self, name: str, file: str | None = None, depends: list["Target"] = None):
        self.name = name
        self.file = file
        self.depends = depends if depends is not None else []
    
    def add_depend(self, *target: "Target"):
        self.depends.extend(target)
    
    def __str__(self):
        if not self.depends:
            if self.file is not None:
                return f"{self.__class__.__name__} {self.name} -> {self.file}"
            else:
                return f"{self.__class__.__name__} {self.name}"
        else:
            if self.file is not None:
                return f"{self.__class__.__name__} {self.name}: {' '.join(map(lambda t: t.name, self.depends))} -> {self.file}"
            else:
                return f"{self.__class__.__name__} {self.name}: {' '.join(map(lambda t: t.name, self.depends))}"
    
    def __repr__(self):
        return f"<{str(self)}>"
    
    def build(self):
        if skip_exist:
            if self.file is not None:
                if os.path.exists(self.file):
                    report(f"target {self.name}: {self.file} already exists, skipping")
                    return
        for depend in self.depends:
            report(f"target {self.name}: building dependency {depend.name}")
            depend.build()
        self.call()
    
    def call(self) -> None:
        pass

class ObjectFile(Target):
    default_flags = ("-O2",
                     "-Wall",
                     "-fPIC", 
                     "-fvisibility=hidden",
                     "-DCP_VERSION_HEX="+hex(version_hex).upper(),
                     "-Lbuild")
    def __init__(self, source: str | list[str], objname: str | None = None, flags: tuple[str] = (),
                 depends: list["Target"] = None, *, name: str = None):
        if objname is None:
            objname = os.path.splitext(os.path.basename(source))[0] + ".o"
        name = name if name is not None else objname
        file = os.path.join("build", objname)
        super().__init__(name, file, depends)
        self.flags = flags
        self.source = source if isinstance(source, list) else [source]
        self.objname = objname
    
    def call(self):
        gcc("gcc", "-c", *self.source, "-o", self.file, *self.flags, *self.default_flags)

class Archive(Target):
    default_flags = ()
    def __init__(self, objects: list[str], libname: str, flags: tuple[str] = (), 
                 depends: list["Target"] = None, *, name: str = None):
        libname = "lib" + libname + ".a"
        file = os.path.join("build", libname)
        name = name if name is not None else libname
        super().__init__(name, file, depends)
        self.flags = flags
        self.objects = objects
        self.libname = libname
    
    def call(self):
        gcc("ar", "rcs", self.file, *self.objects, *self.flags, *self.default_flags)

class SharedObject(Target):
    default_flags = ()
    def __init__(self, objects: list[str], soname: str, flags: tuple[str] = (), 
                 depends: list["Target"] = None, *, name: str = None):
        name = "lib" + soname + ".so" if name is None else name
        if sys.platform == "win32":
            filename = "lib" + soname + ".dll"
        else:
            filename = "lib" + soname + ".so"
        file = os.path.join("build", filename)
        super().__init__(name, file, depends)
        self.flags = flags
        self.objects = objects
        self.filename = filename
        self.soname = soname
    
    def call(self):
        if sys.platform == "win32":
            gcc("gcc", "-shared", "-o", self.file, *self.objects, 
                "-Wl,--out-implib," + self.file + ".a", *self.flags, *self.default_flags)
        else:
            gcc("gcc", "-shared", "-o", self.file, *self.objects, *self.flags, *self.default_flags)

class Executable(Target):
    default_flags = ("-Lbuild",)
    def __init__(self, objects: list[str], exename: str, flags: tuple[str] = (), 
                 depends: list["Target"] = None, *, name: str = None):
        name = exename if name is None else name
        if sys.platform == "win32":
            exename += ".exe"
        file = os.path.join("build", exename)
        super().__init__(name, file, depends)
        self.flags = flags
        self.objects = objects
        self.exename = exename
    
    def call(self):
        gcc("gcc", "-o", self.file, *self.objects, *self.flags, *self.default_flags)

# Implement targets

class CleanAll(Target):
    def __init__(self):
        super().__init__("clean")
    
    def call(self):
        for f in os.listdir("build"):
            path = os.path.join("build", f)
            if os.path.isfile(path):
                os.remove(path)
            else:
                shutil.rmtree(path)

class Clean(Target):
    def __init__(self):
        super().__init__("clean-object")
    
    def call(self):
        for f in os.listdir("build"):
            path = os.path.join("build", f)
            if os.path.isfile(path) and os.path.splitext(f)[1] == ".o":
                os.remove(path)

def main():
    builder = Builder()
    objects: list[ObjectFile] = []
    # No object file yet
    builder.add_target(*objects)
    shared = SharedObject(
        [obj.file for obj in objects],
        "cp_v" + major,
        depends=objects,
        name="cp.so"
    )
    builder.add_target(shared)
    # No launcher source file yet
    #executable = Executable(
    #    [launcher.file],
    #    "cp",
    #    ("-l"+shared.soname,),
    #    depends=[shared]
    #)
    #builder.add_target(executable)

    all = Target("all")
    all.add_depend(*builder.targets)
    builder.add_target(all)

    cleanall = CleanAll()
    builder.add_target(cleanall)

    clean = Clean()
    builder.add_target(clean)
    builder.build()

if __name__ == "__main__":
    os.chdir(os.path.dirname(__file__))
    main()