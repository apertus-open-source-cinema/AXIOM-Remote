from typing import Optional, Any

class ProtocolError(Exception):
    def __init__(self, code: str, detail: Optional[str] = None):
        self.code = code
        self.detail = detail

    def response(self):
        if self.detail:
            return f"ERR {self.code} {self.detail}"
        return f"ERR {self.code}"

def parse_value(param: Any, raw: str):
    if param.type == "int":
        try:
            v = int(raw)
        except ValueError:
            raise ProtocolError("invalid_value")
    elif param.type == "Float":
        try:
            v = float(raw)
        except ValueError:
            raise ProtocolError("invalid_value")
    elif param.type == "enum":
        if raw not in param.values:
            raise ProtocolError("invalid_enum")
        return raw
    else:
        raise ProtocolError("internal", "unknown_type")

    if param.min is not None and v < param.min:
        raise ProtocolError("out_of_range")
    if param.max is not None and v > param.max:
        raise ProtocolError("out_of_range")

    return v


def parse_range(range_str: str):
    """Parse a range string like "0 10 1" into (min, max, step) floats.

    Returns a tuple (min, max, step) where any of the values may be None if not present
    or if the input is falsy. Raises ValueError on malformed numeric values.
    """
    if not range_str:
        return None, None, None

    parts = range_str.split()
    if not parts:
        return None, None, None

    try:
        p_min = float(parts[0]) if len(parts) >= 1 else None
        p_max = float(parts[1]) if len(parts) >= 2 else None
        p_step = float(parts[2]) if len(parts) >= 3 else None
    except ValueError as e:
        raise ValueError("malformed range") from e

    return p_min, p_max, p_step
