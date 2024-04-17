```mermaid
flowchart TD

O(Object)
T3D[Transform3D]

O --> N(Node)
N --> Pvt(Pivot)
Pvt --> Cam(Camera)
Cam --> CamP(PerspectiveCamera)
Cam --> CamO(OrthographicCamera)
Pvt --> L(Light)
L --> LAmb(AmbientLight)
L --> LPnt(PointLight)
L --> LCst(CastingLight)
LCst --> LDir(DirectionalLight)
LCst --> LSpt(SpotLight)
Pvt --> Scn(Scene)
Pvt --> Skl(Skeleton)

N --> Msh(Mesh)
N --> Skm(SkinnedMesh)
N --> Skb(Skybox)

O --> Mat(Material)
O --> Tex(Texture)

Msh -.-> Tex
```
