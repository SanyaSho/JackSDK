## Usage of Valve's leaked code and licensing issues inside the Steam version of J.A.C.K.
- All findings were done on the latest actual J.A.C.K. version 1.2.4603

### (Jack) CMapFace::InitializeTextureAxes -> CMapFace::calcTextureAxes
- Direct copy of leaked code.
- Almost 1 to 1 copy of texture axes calculation function.
![Leaked code inside Jack executable](.github/evidence/1_jack.png)
![Leaked worldcraft code](.github/evidence/1_hammer.png)

### (vpHalfLife) BuildGammaTable (Unknown name) -> Quake BuildGammaTable
- Licensing issue.
- Function has the same behavior and uses the same gamma table calculation formula.
![GPL-licensed code inside vpHalfLife plugin code](.github/evidence/2_jack.png)
![Quake 1 source code from IdSoftware's GitHub](.github/evidence/2_quake.png)

### (Jack) Almost identical copy of build_number function from QuakeWorld
- Licensing issue.
- Function uses the same constants, the same month table but it does not include build time.
![GPL-licensed code inside J.A.C.K.'s build_number function (original function name is unknown)](.github/evidence/3_jack.png)
![QuakeWorld source code from IdSoftware's GitHub](.github/evidence/3_quakeworld.png)

### (Jack) Stolen GL_State function from Quake III Arena
- Licensing issue.
- Function uses the same constants, but it was extended a bit and it does not have a way to disable DEPTH_TEST.
![GPL-licensed code inside J.A.C.K.'s CGLState::setState function](.github/evidence/4_jack.png)
![Quake III Arena source code from IdSoftware's GitHub](.github/evidence/4_quake3.png)

### (vpHalfLife) StudioModel Chrome calculation function
- Licensing issue.
- Almost identical copy to StudioModel::Chrome function from Valve's HLMV.
![HLMV code inside vpHalfLife](.github/evidence/5_jack.png)
![HLMV source code from ValveSoftware's GitHub](.github/evidence/5_hlmv.png)
