xof 0303txt 0032

template Frame {
  <3d82ab46-62da-11cf-ab39-0020af71e433>
  [...]
}

template Matrix4x4 {
  <f6f23f45-7686-11cf-8f52-0040333594a3>
  array FLOAT matrix[16];
}

template FrameTransformMatrix {
  <f6f23f41-7686-11cf-8f52-0040333594a3>
  Matrix4x4 frameMatrix;
}

template Vector {
  <3d82ab5e-62da-11cf-ab39-0020af71e433>
  FLOAT x;
  FLOAT y;
  FLOAT z;
}

template MeshFace {
  <3d82ab5f-62da-11cf-ab39-0020af71e433>
  DWORD nFaceVertexIndices;
  array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
  <3d82ab44-62da-11cf-ab39-0020af71e433>
  DWORD nVertices;
  array Vector vertices[nVertices];
  DWORD nFaces;
  array MeshFace faces[nFaces];
  [...]
}

template MeshNormals {
  <f6f23f43-7686-11cf-8f52-0040333594a3>
  DWORD nNormals;
  array Vector normals[nNormals];
  DWORD nFaceNormals;
  array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
  <f6f23f44-7686-11cf-8f52-0040333594a3>
  FLOAT u;
  FLOAT v;
}

template MeshTextureCoords {
  <f6f23f40-7686-11cf-8f52-0040333594a3>
  DWORD nTextureCoords;
  array Coords2d textureCoords[nTextureCoords];
}

template ColorRGBA {
  <35ff44e0-6c7c-11cf-8f52-0040333594a3>
  FLOAT red;
  FLOAT green;
  FLOAT blue;
  FLOAT alpha;
}

template IndexedColor {
  <1630b820-7842-11cf-8f52-0040333594a3>
  DWORD index;
  ColorRGBA indexColor;
}

template MeshVertexColors {
  <1630b821-7842-11cf-8f52-0040333594a3>
  DWORD nVertexColors;
  array IndexedColor vertexColors[nVertexColors];
}

template VertexElement {
  <f752461c-1e23-48f6-b9f8-8350850f336f>
  DWORD Type;
  DWORD Method;
  DWORD Usage;
  DWORD UsageIndex;
}

template DeclData {
  <bf22e553-292c-4781-9fea-62bd554bdd93>
  DWORD nElements;
  array VertexElement Elements[nElements];
  DWORD nDWords;
  array DWORD data[nDWords];
}

Frame DXCC_ROOT {
  FrameTransformMatrix {
     1.00000000, 0.00000000, 0.00000000, 0.00000000,
    0.00000000, 1.00000000, 0.00000000, 0.00000000,
    0.00000000, 0.00000000, 1.00000000, 0.00000000,
    0.00000000, 0.00000000, 0.00000000, 1.00000000;;
  }

  Frame RootNode {
    FrameTransformMatrix {
       1.00000000, 0.00000000, -0.00000000, 0.00000000,
      0.00000000, 1.00000000, -0.00000000, 0.00000000,
      -0.00000000, -0.00000000, 1.00000000, -0.00000000,
      0.00000000, 0.00000000, -0.00000000, 1.00000000;;
    }

    Frame Cube {
      FrameTransformMatrix {
         60.00000381, 0.00000000, -0.00000000, 0.00000000,
        0.00000000, -0.00000978, 60.00000381, 0.00000000,
        -0.00000000, -6.00000191, -0.00000098, -0.00000000,
        0.00000000, 0.00000000, -0.00000000, 1.00000000;;
      }

      Mesh Cube_001_mShape {
        24;
        -1.00000000;-1.00000000;1.00000000;,
        -1.00000000;-1.00000000;-1.00000000;,
        -1.00000000;1.00000000;-1.00000000;,
        -1.00000000;1.00000000;1.00000000;,
        -1.00000000;1.00000000;1.00000000;,
        -1.00000000;1.00000000;-1.00000000;,
        1.00000000;1.00000000;-1.00000000;,
        1.00000000;1.00000000;1.00000000;,
        1.00000000;1.00000000;1.00000000;,
        1.00000000;1.00000000;-1.00000000;,
        1.00000000;-1.00000000;-1.00000000;,
        1.00000000;-1.00000000;1.00000000;,
        1.00000000;-1.00000000;1.00000000;,
        1.00000000;-1.00000000;-1.00000000;,
        -1.00000000;-1.00000000;-1.00000000;,
        -1.00000000;-1.00000000;1.00000000;,
        -1.00000000;1.00000000;1.00000000;,
        1.00000000;1.00000000;1.00000000;,
        1.00000000;-1.00000000;1.00000000;,
        -1.00000000;-1.00000000;1.00000000;,
        1.00000000;1.00000000;-1.00000000;,
        -1.00000000;1.00000000;-1.00000000;,
        -1.00000000;-1.00000000;-1.00000000;,
        1.00000000;-1.00000000;-1.00000000;;
        6;
        4;3,2,1,0;,
        4;7,6,5,4;,
        4;11,10,9,8;,
        4;15,14,13,12;,
        4;19,18,17,16;,
        4;23,22,21,20;;

        MeshMaterialList {
          1;
          6;
          0, 0, 0, 0, 0, 0;
          Material {
            1.0; 1.0; 1.0; 1.000000;;
            1.000000;
            0.000000; 0.000000; 0.000000;;
            0.000000; 0.000000; 0.000000;;
            TextureFilename { "Sand.png"; }
          }
        }

        MeshNormals {
        24;
        1.00000000;-0.00000000;0.00000000;,
        1.00000000;-0.00000000;0.00000000;,
        1.00000000;-0.00000000;0.00000000;,
        1.00000000;-0.00000000;0.00000000;,
        -0.00000000;-1.00000000;0.00000000;,
        -0.00000000;-1.00000000;0.00000000;,
        -0.00000000;-1.00000000;0.00000000;,
        -0.00000000;-1.00000000;0.00000000;,
        -1.00000000;-0.00000000;0.00000000;,
        -1.00000000;-0.00000000;0.00000000;,
        -1.00000000;-0.00000000;0.00000000;,
        -1.00000000;-0.00000000;0.00000000;,
        -0.00000000;1.00000000;0.00000000;,
        -0.00000000;1.00000000;0.00000000;,
        -0.00000000;1.00000000;0.00000000;,
        -0.00000000;1.00000000;0.00000000;,
        -0.00000000;-0.00000000;-1.00000000;,
        -0.00000000;-0.00000000;-1.00000000;,
        -0.00000000;-0.00000000;-1.00000000;,
        -0.00000000;-0.00000000;-1.00000000;,
        -0.00000000;-0.00000000;1.00000000;,
        -0.00000000;-0.00000000;1.00000000;,
        -0.00000000;-0.00000000;1.00000000;,
        -0.00000000;-0.00000000;1.00000000;;
        6;
        4;3,2,1,0;,
        4;7,6,5,4;,
        4;11,10,9,8;,
        4;15,14,13,12;,
        4;19,18,17,16;,
        4;23,22,21,20;;
        }

        MeshTextureCoords {
        24;
        0.37500000;1.00000000;,
        0.62500000;1.00000000;,
        0.62500000;0.75000000;,
        0.37500000;0.75000000;,
        0.37500000;0.75000000;,
        0.62500000;0.75000000;,
        0.62500000;0.50000000;,
        0.37500000;0.50000000;,
        0.37500000;0.50000000;,
        0.62500000;0.50000000;,
        0.62500000;0.25000000;,
        0.37500000;0.25000000;,
        0.37500000;0.25000000;,
        0.62500000;0.25000000;,
        0.62500000;0.00000000;,
        0.37500000;0.00000000;,
        0.12500000;0.50000000;,
        0.37500000;0.50000000;,
        0.37500000;0.25000000;,
        0.12500000;0.25000000;,
        0.62500000;0.50000000;,
        0.87500000;0.50000000;,
        0.87500000;0.25000000;,
        0.62500000;0.25000000;;
        }
      }

    }

  }

}
