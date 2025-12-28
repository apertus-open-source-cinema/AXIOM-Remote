import logging
from pathlib import Path

log = logging.getLogger(__name__)


class FSBackend:
    def __init__(self):
        self.parameters = {}

    def scan_parameters(self, parameters, search_base_path):
        """
        Scans filesystem for parameters in search_base_path.
        If parameters is None, scans all directories.
        """
        errors = []

        base = Path(search_base_path)
        if not base.exists():
            errors.append(f"Search base path does not exist: {search_base_path}")
            return {}, errors

        if parameters:
            log.info(f"Discovering parameters named {parameters} in {search_base_path}...")
        else:
            log.info(f"Discovering all parameters in {search_base_path}...")

        found_paths = self._discover_paths(base, parameters)
        log.info(f"Found {len(found_paths)} matching parameter paths.")
        return self._scan_paths(found_paths), errors

    def _discover_paths(self, search_base: Path, names_to_find):
        found_paths = []
        names_set = set(names_to_find) if names_to_find else None

        for entry in search_base.rglob("*"):
            if not entry.is_dir():
                continue

            # Skip any directory under a 'raw' folder
            if "raw" in entry.parts:
                continue

            if names_set and entry.name not in names_set:
                continue

            if (entry / "value").is_file():
                found_paths.append(str(entry))

        return found_paths

    def _scan_paths(self, parameter_paths):
        log.info(f"Scanning for parameters from {len(parameter_paths)} paths...")
        self.parameters.clear()

        for param_path in parameter_paths:
            p = Path(param_path)
            if p.is_dir():
                self._load_parameter(p)
            else:
                log.warning(f"Parameter path not found or not a directory: {param_path}")

        return self.parameters

    def _read_text_file(self, path: Path):
        try:
            return path.read_text().strip()
        except (FileNotFoundError, IsADirectoryError, PermissionError):
            return None

    def _load_parameter(self, param_path: Path):
        param_path_str = str(param_path)
        param_data = {"path": param_path_str}

        # Define the properties to look for
        properties_map = {
            "value": "value",
            "type": "ty",
            "writable": "writable",
            "range": "range",
            "map": "map",
        }

        for prop, filename in properties_map.items():
            file_path = param_path / filename
            content = self._read_text_file(file_path)

            if content is None:
                # Set defaults for missing files
                param_data[prop] = False if prop == "writable" else None
                continue

            if prop == "writable":
                param_data[prop] = content.lower() in ("1", "true", "yes")
            elif prop == "map":
                param_data[prop] = content.splitlines()
            else:
                param_data[prop] = content

        self.parameters[param_path_str] = param_data
        log.debug(f"Loaded parameter '{param_path_str}': {param_data}")


class CameraBackend:
    def get(self, param):
        path = Path(param.path) / "value"
        try:
            return path.read_text().strip()
        except FileNotFoundError:
            log.error(f"Value file not found for {param.name} at {path}")
            return None

    def set(self, param, value):
        path = Path(param.path) / "value"
        try:
            path.write_text(str(value))
        except IOError as e:
            log.error(f"Failed to write value for {param.name} at {path}: {e}")
            raise
