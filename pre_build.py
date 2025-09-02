Import("env")
import os
from pathlib import Path


def load_env_vars(source, target, env):
    env_file = Path(".env")
    if env_file.exists():
        with open(env_file, "r") as f:
            for line in f:
                if "=" in line and not line.startswith("#"):
                    key, value = line.strip().split("=", 1)
                    value = value.strip('"').strip("'")
                    env.Append(CPPDEFINES=[(key, f'\\"{value}\\"')])
                    print(f"Loaded: {key}")


env.AddPreAction("buildprog", load_env_vars)
