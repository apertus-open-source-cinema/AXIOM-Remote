from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional
import logging
from utils import ProtocolError, parse_value, parse_range
from fs_backend import CameraBackend


@dataclass
class Param:
    name: str
    type: str
    path: Path                # ← filesystem location
    rw: bool = True
    min: Optional[float] = None
    max: Optional[float] = None
    step: Optional[float] = None
    unit: Optional[str] = None
    values: Optional[List[str]] = None
    
    
class ParamManager:
    def __init__(self, params: dict[str, Param], backend: CameraBackend):
        self.params = params
        self.backend = backend
        self.log = logging.getLogger(__name__)

    @classmethod
    def from_parameter_db(cls, parameter_db: dict, backend: CameraBackend):
        """Create a ParamManager from the raw parameter database returned by FSBackend."""
        params: dict[str, Param] = {}
        for path_str, data in parameter_db.items():
            name = Path(path_str).name
            p_type = data.get("type", "string")

            # Parse range using helper
            p_min = p_max = p_step = None
            range_str = data.get("range")
            if range_str:
                try:
                    p_min, p_max, p_step = parse_range(range_str)
                except ValueError:
                    logging.getLogger(__name__).warning(
                        f"Failed to parse range for {name}: {range_str}"
                    )

            p = Param(
                name=name,
                type=p_type,
                path=Path(path_str),
                rw=data.get("writable", False),
                min=p_min,
                max=p_max,
                step=p_step,
                values=data.get("map"),
            )
            params[name] = p

        return cls(params, backend)

    def get(self, name: str):
        p = self.get_param(name)
        raw = self.backend.get(p)
        return parse_value(p, raw)

    def set(self, name: str, raw_value: str):
        p = self.get_param(name)

        value = parse_value(p, raw_value)

        # cross-parameter rules
        if name == "focus_pos":
            if self.get("focus_mode") != "manual":
                raise ProtocolError("invalid_state")

        self.backend.set(p, value)

    def get_param(self, name: str) -> Param:
        if name not in self.params:
            raise ProtocolError("unknown_param")
        return self.params[name]