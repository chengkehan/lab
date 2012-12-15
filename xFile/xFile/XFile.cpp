/***************************************************************************************
PURPOSE:
    XFile/Skinned Mesh Demo

Required libraries:
  WINMM.LIB, D3D9.LIB, D3DX9.LIB, DXGUID.LIB.
 ***************************************************************************************/

#include <windows.h>
#include <stdio.h>
#include "d3d9.h"
#include "d3dx9.h"
#include "DxFile.h"
#include "RmxfGuid.h"
#include "RmxfTmpl.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")

#pragma warning(disable : 4305 4996)

#define WINDOW_WIDTH    400
#define WINDOW_HEIGHT   400

#define Safe_Release(p) if(p) (p)->Release();
#define Release_Com(p)  if(p)  { (p)->Release(); (p) = NULL; }

// window handles, class and caption text.
HWND g_hwnd;
HINSTANCE g_inst;
static char g_class_name[] = "XFileClass";
static char g_caption[]    = "XFile Demo";

// the Direct3D and device object
IDirect3D9* g_d3d = NULL;
IDirect3DDevice9* g_d3d_device = NULL;

//------------------------------------------------------------------
// A Mesh definition structure
//------------------------------------------------------------------
typedef struct MESH
{
    char*               m_name;             // name of mesh

    ID3DXMesh*          m_mesh;             // mesh object
    ID3DXMesh*          m_skinmesh;         // skin mesh object
    ID3DXSkinInfo*      m_skininfo;         // skin information

    DWORD               m_num_materials;    // number of materails in mesh
    D3DMATERIAL9*       m_materials;        // array of materials
    IDirect3DTexture9** m_textures;         // array of textures    

    // clear all structure data
    MESH()
    {
        m_name  = NULL;     

        m_mesh      = NULL;
        m_skinmesh  = NULL;
        m_skininfo  = NULL;

        m_num_materials = 0;
        m_materials     = NULL;
        m_textures      = NULL;               
    }

    // free all used resources
    ~MESH()
    {        
        delete[] m_name;
        m_name = NULL;

        Release_Com(m_mesh);
        Release_Com(m_skinmesh);
        Release_Com(m_skininfo);

        delete[] m_materials;
        m_materials = NULL;
        
        // release all textures resource
        if(m_textures != NULL)
        {
            for(DWORD i = 0; i < m_num_materials; i++)
                Release_Com(m_textures[i]);

            delete[] m_textures;
            m_textures = NULL;
        }
    }

} MESH;

//------------------------------------------------------------------
// Structure to contain frame information
//------------------------------------------------------------------
typedef struct FRAME
{
    char*   m_name;     // frame's name
    MESH*   m_mesh;     // linked list of meshes    
    FRAME*  m_child;    // child frame

    FRAME()
    {
        // clear all data
        m_name = NULL;
        m_mesh = NULL;
        m_child = NULL;
    }

    ~FRAME()
    {
        // delete all used resources, including linked list of frames.
        delete[] m_name;    m_name    = NULL;
        delete m_mesh;      m_mesh    = NULL;
        delete m_child;     m_child   = NULL;        
    }

} FRAME;

// parent frame for .X file
FRAME* g_parent_frame = NULL;

//--------------------------------------------------------------------------------
// Window procedure.
//--------------------------------------------------------------------------------
long WINAPI Window_Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return (long) DefWindowProc(hwnd, msg, wParam, lParam);
}

//--------------------------------------------------------------------------------
// Parse specified xfiel data, recursive function.
//--------------------------------------------------------------------------------
void Parse_XFile_Data(ID3DXFileData* xfile_data, FRAME* parent_frame)
{
    ID3DXFileData*  sub_xfile_data = NULL;
    ID3DXBuffer*    adjacency = NULL;

    GUID  type;
    char* name = NULL;
    DWORD size;

    MESH* mesh = NULL;
    ID3DXBuffer* material_buffer = NULL;
    D3DXMATERIAL* materials = NULL;

    // get the template type
    // retrieves the globally unique identifier (GUID) of the object's template
    if(FAILED(xfile_data->GetType(&type)))
        return;

    // get the template name (if any)
    // retrieves a pointer to a microsoft directX file object's name
    if(FAILED(xfile_data->GetName(NULL, &size)))
        return;

    if(size != 0)
    {
        if((name = new char[size]) != NULL)
            xfile_data->GetName(name, &size);
    }

    // give template a default name if none found
    if(name == NULL)
    {
        if((name = new char[9]) == NULL)
            return;

        strcpy(name, "Template");
    }

    // set sub frame
    FRAME* sub_frame = parent_frame;

    // process the templates
    FRAME* frame = NULL;

    if(type == TID_D3DRMFrame)  // it is a frame
    {
        // create a new frame structure
        frame = new FRAME();

        // store the name
        frame->m_name = name;
        name = NULL;

        // add to parent frame
        parent_frame->m_child = frame;

        // set sub frame parent
        sub_frame = frame;
    }
    else if(type == TID_D3DRMMesh)  // it is a mesh
    {
        // create a new mesh structure
        mesh = new MESH();

        // store the name
        mesh->m_name = name;
        name = NULL;

        // load mesh data (as a skinned mesh)
        // loads a skin mesh from microsoft directX .x file data object
        if(FAILED(D3DXLoadSkinMeshFromXof(xfile_data, 0, g_d3d_device, &adjacency, &material_buffer, NULL,
            &mesh->m_num_materials, &mesh->m_skininfo, &mesh->m_mesh)))
        {
            delete[] name;
            delete mesh;
            return;
        }

        Release_Com(adjacency);

        // clone skin mesh if bones exist
        if(mesh->m_skininfo != NULL && mesh->m_skininfo->GetNumBones() != 0)
        {
            // clones a mesh using a flexible vertex format (FVF) code
            if(FAILED(mesh->m_mesh->CloneMeshFVF(0, mesh->m_mesh->GetFVF(), g_d3d_device, &mesh->m_skinmesh)))
            {
                mesh->m_skininfo->Release();
                mesh->m_skininfo = NULL;
            }
        }

        // load materials or create a default one if none
        if(mesh->m_num_materials == 0)
        {
            // create a default one
            mesh->m_materials = new D3DMATERIAL9[1];
            mesh->m_textures  = new LPDIRECT3DTEXTURE9[1];

            ZeroMemory(mesh->m_materials, sizeof(D3DMATERIAL9));

            mesh->m_materials[0].Diffuse.r = 1.0;
            mesh->m_materials[0].Diffuse.g = 1.0;
            mesh->m_materials[0].Diffuse.b = 1.0;
            mesh->m_materials[0].Diffuse.a = 1.0;
            mesh->m_materials[0].Ambient   = mesh->m_materials[0].Diffuse;
            mesh->m_materials[0].Specular  = mesh->m_materials[0].Diffuse;

            mesh->m_textures[0] = NULL;

            mesh->m_num_materials = 1;
        }
        else
        {
            // load the materials
            materials = (D3DXMATERIAL*) material_buffer->GetBufferPointer();

            mesh->m_materials = new D3DMATERIAL9[mesh->m_num_materials];
            mesh->m_textures  = new LPDIRECT3DTEXTURE9[mesh->m_num_materials];

            // set materials and textures for mesh
            for(DWORD i = 0; i < mesh->m_num_materials; i++)
            {
                mesh->m_materials[i] = materials[i].MatD3D;
                mesh->m_materials[i].Ambient = mesh->m_materials[i].Diffuse;

                // build a texture path and load it
                if(FAILED(D3DXCreateTextureFromFile(g_d3d_device, materials[i].pTextureFilename, &mesh->m_textures[i])))
                    mesh->m_textures[i] = NULL;
            }
        }

        Release_Com(material_buffer);

        // set mesh to parent frame
        parent_frame->m_mesh = mesh;
    }   // end if(type == TID_D3DRMMesh)
    else if(type == TID_D3DRMAnimationSet || type == TID_D3DRMAnimation || type == TID_D3DRMAnimationKey)
    {
        // skip animation sets and animations
        delete[] name;
        return;
    }

    // release name buffer
    delete[] name;

    SIZE_T num_child;
    xfile_data->GetChildren(&num_child);

    // scan for embedded templates
    for(SIZE_T i = 0; i < num_child; i++)
    {
        xfile_data->GetChild(i, &sub_xfile_data);

        // process embedded xfile data, recursive call.
        Parse_XFile_Data(sub_xfile_data, sub_frame);

        Release_Com(sub_xfile_data);
    }
}

//--------------------------------------------------------------------------------
// Parse x file, and return root frame.
//--------------------------------------------------------------------------------
FRAME* Parse_XFile(char* filename)
{
    ID3DXFile* xfile = NULL;
    ID3DXFileEnumObject* xfile_enum = NULL;
    ID3DXFileData* xfile_data = NULL;

    // create the file object
    if(FAILED(D3DXFileCreate(&xfile)))
        return NULL;

    // register the templates
    if(FAILED(xfile->RegisterTemplates((LPVOID) D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
    {
        xfile->Release();
        return NULL;
    }

    // create an enumerator object that will read a .x file
    if(FAILED(xfile->CreateEnumObject((LPVOID) filename, DXFILELOAD_FROMFILE, &xfile_enum)))
    {
        xfile->Release();
        return NULL;
    }

    // allocate a frame that becomes root
    FRAME* frame = new FRAME();

    SIZE_T num_child;

    // retrieve the number of children in this file data object
    xfile_enum->GetChildren(&num_child);

    // loop through all objects looking for the frames and meshes
    for(SIZE_T i = 0; i < num_child; i++)
    {
        // retrieves a child object in this file data object
        if(FAILED(xfile_enum->GetChild(i, &xfile_data)))
            return NULL;

        // parse xfile data
        Parse_XFile_Data(xfile_data, frame);

        Release_Com(xfile_data);
    }

    // release xfile enumerator object and xfile object
    Release_Com(xfile_enum);
    Release_Com(xfile);

    // return root frame
    return frame;
}

//--------------------------------------------------------------------------------
// Initialize d3d, d3d device, vertex buffer; set render state for d3d;
// set perspective matrix and view matrix, load xfile.
//--------------------------------------------------------------------------------
BOOL Do_Init()
{
    D3DPRESENT_PARAMETERS present_param;
    D3DDISPLAYMODE  display_mode;
    D3DXMATRIX mat_proj, mat_view;

    // do a windowed mode initialization of Direct3D
    if((g_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return FALSE;

    // retrieves the current display mode of the adapter
    if(FAILED(g_d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode)))
        return FALSE;

    ZeroMemory(&present_param, sizeof(present_param));

    // initialize d3d presentation parameter
    present_param.Windowed               = TRUE;
    present_param.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    present_param.BackBufferFormat       = display_mode.Format;
    present_param.EnableAutoDepthStencil = TRUE;
    present_param.AutoDepthStencilFormat = D3DFMT_D16;

    // creates a device to represent the display adapter
    if(FAILED(g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present_param, &g_d3d_device)))
        return FALSE;     

    // set render state

    // disable d3d lighting
    g_d3d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    // enable z-buffer
    g_d3d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

    // create and set the projection matrix

    // builds a left-handed perspective projection matrix based on a field of view
    D3DXMatrixPerspectiveFovLH(&mat_proj, D3DX_PI/4.0, 1.33333, 1.0, 1000.0);

    // sets a single device transformation-related state
    g_d3d_device->SetTransform(D3DTS_PROJECTION, &mat_proj);

    // create and set the view matrix
    D3DXMatrixLookAtLH(&mat_view, 
                       &D3DXVECTOR3(0.0, 50.0, -150.0),
                       &D3DXVECTOR3(0.0, 50.0,  0.0), 
                       &D3DXVECTOR3(0.0, 1.0,   0.0));

    g_d3d_device->SetTransform(D3DTS_VIEW, &mat_view);

    // load a skinned mesh from an .X file
    g_parent_frame = Parse_XFile("warrior.x");

    return TRUE;
}

//--------------------------------------------------------------------------------
// Release all d3d resource.
//--------------------------------------------------------------------------------
BOOL Do_Shutdown()
{
    delete g_parent_frame;

    Safe_Release(g_d3d_device);
    Safe_Release(g_d3d);

    return TRUE;
}

//--------------------------------------------------------------------------------
// Draw current frame, recursive function.
//--------------------------------------------------------------------------------
void Draw_Frame(FRAME* frame)
{
    MESH* mesh;
    D3DXMATRIX* matrices = NULL;
    ID3DXMesh* mesh_to_draw;

    // return if no frame
    if(frame == NULL)
        return;

    // draw meshes if any in frame
    if((mesh = frame->m_mesh) != NULL)
    {
        // setup pointer to mesh to draw
        mesh_to_draw = mesh->m_mesh;

        // generate mesh from skinned mesh to draw with
        if(mesh->m_skinmesh != NULL && mesh->m_skininfo != NULL)
        {
            DWORD num_bones = mesh->m_skininfo->GetNumBones();

            // allocate an array of matrices to orient bones
            matrices = new D3DXMATRIX[num_bones];

            // set all bones orientation to identity
            for(DWORD i = 0; i < num_bones; i++)
                D3DXMatrixIdentity(&matrices[i]);

            // lock source and destination vertex buffers

            void* source = NULL;
            void* dest = NULL;

            // locks a vertex buffer and obtains a pointer to the vertex buffer memory
            mesh->m_mesh->LockVertexBuffer(0, &source);
            mesh->m_skinmesh->LockVertexBuffer(0, &dest);

            // update skinned mesh, applies software skinning to the target vertices based on the current matrices.
            mesh->m_skininfo->UpdateSkinnedMesh(matrices, NULL, source, dest);

            // unlock buffers
            mesh->m_skinmesh->UnlockVertexBuffer();
            mesh->m_mesh->UnlockVertexBuffer();

            // point to skin mesh to draw
            mesh_to_draw = mesh->m_skinmesh;
        }

        // render the mesh
        for(DWORD i = 0; i < mesh->m_num_materials; i++)
        {
            // set the materials properties for the device
            g_d3d_device->SetMaterial(&mesh->m_materials[i]);

            // assigns a texture to a stage for a device
            g_d3d_device->SetTexture(0, mesh->m_textures[i]);

            // draw a subset of a mesh
            mesh_to_draw->DrawSubset(i);
        }

        // free array of matrices
        delete[] matrices;
        matrices = NULL;
    }

    // draw child frames, recursively call.
    Draw_Frame(frame->m_child);
}

//--------------------------------------------------------------------------------
// Render a frame.
//--------------------------------------------------------------------------------
BOOL Do_Frame()
{
    D3DXMATRIX mat_world;

    // clear device back buffer
    g_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 64, 128, 255), 1.0f, 0);

    // Begin scene
    if(SUCCEEDED(g_d3d_device->BeginScene()))
    {
        // create and set the world transformation matrix
        // rotate object along y-axis
        D3DXMatrixRotationY(&mat_world, (float) (timeGetTime() / 1000.0));
        
        g_d3d_device->SetTransform(D3DTS_WORLD, &mat_world);              

        // draw frames
        Draw_Frame(g_parent_frame);

        // end the scene
        g_d3d_device->EndScene();
    }

    // present the contents of the next buffer in the sequence of back buffers owned by the device
    g_d3d_device->Present(NULL, NULL, NULL, NULL);

    // release texture
    g_d3d_device->SetTexture(0, NULL);

    return TRUE;
}

//--------------------------------------------------------------------------------
// Main function, routine entry.
//--------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR cmd_line, int cmd_show)
{
    WNDCLASSEX  win_class;
    MSG         msg;

    g_inst = inst;

    // create window class and register it
    win_class.cbSize        = sizeof(win_class);
    win_class.style         = CS_CLASSDC;
    win_class.lpfnWndProc   = Window_Proc;
    win_class.cbClsExtra    = 0;
    win_class.cbWndExtra    = 0;
    win_class.hInstance     = inst;
    win_class.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    win_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
    win_class.hbrBackground = NULL;
    win_class.lpszMenuName  = NULL;
    win_class.lpszClassName = g_class_name;
    win_class.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(! RegisterClassEx(&win_class))
        return FALSE;

    // create the main window
    g_hwnd = CreateWindow(g_class_name, g_caption, WS_CAPTION | WS_SYSMENU, 0, 0,
                          WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, inst, NULL);

    if(g_hwnd == NULL)
        return FALSE;

    ShowWindow(g_hwnd, SW_NORMAL);
    UpdateWindow(g_hwnd);

    // initialize game
    if(Do_Init() == FALSE)
        return FALSE;

    // start message pump, waiting for signal to quit.
    ZeroMemory(&msg, sizeof(MSG));

    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // draw a frame
        if(Do_Frame() == FALSE)
            break;
    }

    // run shutdown function
    Do_Shutdown();

    UnregisterClass(g_class_name, inst);
    
    return (int) msg.wParam;
}





