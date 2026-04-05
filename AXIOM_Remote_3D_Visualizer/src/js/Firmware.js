/**
 * Firmware module wrapper
 * Wraps the emscripten WebAssembly module and exports enums and classes
 */

// Import the compiled firmware module
import axiomRemoteFirmware from "./FW/axiom_remote_firmware.mjs";

// Async function to load the firmware module
export async function loadFirmware(options = {}) {
  const module = await axiomRemoteFirmware({
    locateFile(path) {
      if (path.endsWith(".wasm")) {
        // Using a relative path ensures it works when hosted in a subfolder (like GitHub Pages)
        return "js/FW/" + path;
      }
      return path;
    },
    ...options
  });

  return module;
}
