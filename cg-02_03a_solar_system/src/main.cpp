#include "engine.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

static float fFieldDepth = 10.0f;

class Framework
{
    public:
        Framework()
        {
            m_pEffect = nullptr;
            m_pMesh = nullptr;
            m_pMeshTwo = nullptr;
            m_pMeshThree = nullptr;
            m_pCamera = nullptr;
            m_pWindow = nullptr;
       
            m_directionalLight.Color = Vector3f(0.2f, 0.2f, 0.4f);
            m_directionalLight.AmbientIntensity = 0.7f;
            m_directionalLight.DiffuseIntensity = 0.21f;
            m_directionalLight.Direction = Vector3f(1.0f, -1.0, 0.0);
            
            m_bIsRunning = false;
        }

        ~Framework()
        {
            SAFE_DELETE(m_pMesh);
            SAFE_DELETE(m_pMeshTwo);
            SAFE_DELETE(m_pMeshThree);
            SAFE_DELETE(m_pCamera);
        }

        bool Init()
        {
            glfwSetErrorCallback(Framework::ErrorCallback);
            
            Vector3f vec3Position(0.0f, 0.0f, -10.0f);
            Vector3f vec3Target(0.0f, 0.0f, 1.0f);
            Vector3f vec3Up(0.0f, 1.0f, 0.0f);

            /* Initialize the library */
            if (!glfwInit())
            {
                printf("ERROR: Could not initialize GLFW\n");
                exit(EXIT_FAILURE);
            }

            printf("INFO: Initialized GLFW\n");

            /* Create a windowed mode window and its OpenGL context */
            m_pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sneaker system", NULL, NULL);
            if (!m_pWindow)
            {
                printf("ERROR: Could not create window\n");
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            printf("INFO: Created window\n");

            /* UGLY! But we need framework in here */
            glfwSetWindowUserPointer(m_pWindow, this);
            
            glfwSetKeyCallback(m_pWindow, Framework::KeyCallback);

            /* Make the window's context current */
            glfwMakeContextCurrent(m_pWindow);
            printf("INFO: Created context\n");
            
            /* Initialize GLEW */
            GLenum eError = glewInit();
            if (eError != GLEW_OK)
            {
                printf("ERROR: Could not initialize GLEW: '%s'\n", glewGetErrorString(eError));
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            printf("INFO: Initialized GLEW\n");
            
            m_pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, vec3Position, vec3Target, vec3Up);
            
            m_pEffect = new LightingTechnique();
            if (!m_pEffect->Init()) {
                printf("ERROR: Could not initialize lighting technique\n");
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            m_pEffect->Enable();
            m_pEffect->SetTextureUnit(0);

            // Load meshes
            m_pMesh = new Mesh();
            m_pMesh->LoadMesh("data/foo.dae");

            m_pMeshTwo = new Mesh();
            m_pMeshTwo->LoadMesh("data/house_plant.obj");

            m_pMeshThree = new Mesh();
            m_pMeshThree->LoadMesh("data/ked.3DS");

            return true;
        }

        void Run()
        {
            PointLight pl[2];
            SpotLight sl[2];

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glFrontFace(GL_CW);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            m_bIsRunning = true;

            while (m_bIsRunning) {
                float fRotation = (float)glfwGetTime() * 50.0f;

                m_pCamera->OnRender();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // First point light 
                pl[0].DiffuseIntensity = 0.25f;
                pl[0].Color = Vector3f(1.0f, 0.5f, 0.0f);
                //pl[0].Position = Vector3f(3.0f, 1.0f, fFieldDepth * (cosf(fRotation) + 1.0f) / 2.0f);
                pl[0].Position = Vector3f(3.0f, 1.0f, 2.0f);
                pl[0].Attenuation.Linear = 0.1f;

                // Second point light
                pl[1].DiffuseIntensity = 0.25f;
                pl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
                //pl[1].Position = Vector3f(7.0f, 1.0f, fFieldDepth * (sinf(fRotation) + 1.0f) / 2.0f);
                pl[1].Position = Vector3f(7.0f, 1.0f, 4.0f);
                pl[1].Attenuation.Linear = 0.1f;
                
                // Add point lights
                m_pEffect->SetPointLights(2, pl);

                // First spot light
                sl[0].DiffuseIntensity = 0.9f;
                sl[0].Color = Vector3f(0.0f, 1.0f, 1.0f);
                sl[0].Position = m_pCamera->GetPosition();
                sl[0].Direction = m_pCamera->GetTarget();
                sl[0].Attenuation.Linear = 0.1f;
                sl[0].Cutoff = 10.0f;

                // Second spot light
                sl[1].DiffuseIntensity = 0.75f;
                sl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
                //sl[1].Position = Vector3f(7.0f, 1.0f, fFieldDepth * (sinf(fRotation) + 1.0f) / 2.0f);
                sl[1].Position = Vector3f(7.0f, 1.0f, -2.0f);
                sl[1].Attenuation.Linear = 0.1f;
                
                // Add spot lights
                m_pEffect->SetSpotLights(1, sl);

                // Effects
                m_pEffect->SetDirectionalLight(m_directionalLight);
                m_pEffect->SetEyeWorldPos(m_pCamera->GetPosition());
                m_pEffect->SetMatSpecularIntensity(0.9f);
                m_pEffect->SetMatSpecularPower(0.8f);

                // Pipieline
                Pipeline p;
                p.SetPerspectiveProjection(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

                // First mesh (planetoid)
                Vector3f vec3Pos1(0.0f, 0.0f, 0.0f);
                p.SetWorldPosition(vec3Pos1.x, vec3Pos1.y, vec3Pos1.z);
                p.SetCamera(m_pCamera->GetPosition(), m_pCamera->GetTarget(), m_pCamera->GetUp());
                m_pEffect->SetWVP(p.GetWorldPerspectiveTransformation());
                m_pEffect->SetWorldMatrix(p.GetWorldTransformation());
                m_pMesh->Render();

                // Second  mesh (plant)
                Vector3f vec3Pos2 = vec3Pos1;
                vec3Pos2.x -= 3.0f;
                vec3Pos2.y -= 1.0f;
                vec3Pos2.z += 3.0f;
                p.SetScale(0.03f, 0.03f, 0.03f);
                p.SetWorldPosition(vec3Pos2.x, vec3Pos2.y, vec3Pos2.z);
                //p.SetRotation(0.0f, fRotation, 0.0f);
                p.SetCamera(m_pCamera->GetPosition(), m_pCamera->GetTarget(), m_pCamera->GetUp());
                m_pEffect->SetWVP(p.GetWorldPerspectiveTransformation(true));
                m_pEffect->SetWorldMatrix(p.GetWorldTransformation(true));
                m_pMeshTwo->Render();

                // Third mesh (planetoid)
                Vector3f vec3Pos3 = vec3Pos2;
                vec3Pos3.x += 1.5f;
                vec3Pos3.y += 1.0f;
                p.SetScale(0.1f, 0.1f, 0.1f);
                //p.SetWorldPosition(vec3Pos3.x, vec3Pos3.y, vec3Pos3.z);
                p.SetRotation(0.0f, fRotation, 0.0f);
                p.SetWorldPosition(0.0f, 1.5f, 3.0f);
                Vector3f vec3Foos = m_pCamera->GetPosition();
                vec3Foos.x = vec3Pos1.x + 3.0f;
                p.SetCamera(vec3Foos, m_pCamera->GetTarget(), m_pCamera->GetUp());
                m_pEffect->SetWorldMatrix(p.GetWorldTransformation(true, true));
                m_pEffect->SetWVP(p.GetWorldPerspectiveTransformation(true, true));
                m_pMeshThree->Render();

                glfwSwapBuffers(m_pWindow);
                glfwPollEvents();
            }
        }

        void Terminate()
        {
            glfwDestroyWindow(m_pWindow);
            glfwTerminate();
            exit(EXIT_SUCCESS);
        }

        void SetIsRunning(bool bIsRunning)
        {
            m_bIsRunning = bIsRunning;
        }

    private:
        static void ErrorCallback(int error, const char* description)
        {
            fputs(description, stderr);
        }
        
        static void KeyCallback(GLFWwindow* pWindow, int nKey, int nScancode, int nAction, int nMods)
        {
            /* Unwrap user specific pointer */
            Framework* pFramework = (Framework*)glfwGetWindowUserPointer(pWindow);

            if (nKey == GLFW_KEY_ESCAPE && nAction == GLFW_PRESS) 
            {
                glfwSetWindowShouldClose(pWindow, GL_TRUE);

                if (pFramework)
                {
                    printf("INFO: Set framework to stop, terminating\n");
                    pFramework->SetIsRunning(false);
                }

            }
        }

        LightingTechnique* m_pEffect;
        DirectionalLight m_directionalLight;
        Mesh* m_pMesh;
        Mesh* m_pMeshTwo;
        Mesh* m_pMeshThree;
        Camera* m_pCamera;
        GLFWwindow* m_pWindow;


        bool m_bIsRunning;
};


int main(void)
{
    Framework* pFramework = new Framework();

    if (!pFramework->Init()) {
        return 1;
    }

    pFramework->Run();

    SAFE_DELETE(pFramework);

    return 0;
}
