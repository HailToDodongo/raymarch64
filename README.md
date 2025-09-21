# Raymarcher64

This demo implements a basic ray-marcher running on the CPU and RSP.
The CPU does the basic loop per pixel incl. ray construction and the final shading,
whereas the RSP does the loop per ray to determine the distance.
Since i pre-run the first ray, CPU & RSP can run mostly in parallel.

SDFs on the RSP are baked into the ray-loop for performance reasons, so each SDF is a copy of the entire loop.
The CPU does the same to save a few instructions.
In the end, a set of constexpr-structs define what functions a scene will use.

Since the shading is done on the CPU, there is a bit more freedom for effects.
For example texturing can be done with any texture size and generated UVs.
The textures on models are 256x256 RGBA16, and the skybox a whopping 1024x512 pixel (equirectangular mapped even).

## Build

Install the latest libdragon `preview` branch:
<https://github.com/DragonMinded/libdragon/tree/preview>

Then in this project run:
```sh
make
```

This will create a `raymarcher.z64` file.

> **Note**<br>
> Running this ROM requires real hardware or an accurate emulator.
> For emulators ares or gopher64 are recommended.
