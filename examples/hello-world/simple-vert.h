char vShaderStr[] =
  "precision mediump float;\n"
  "attribute vec4 a_position;\n"
  "attribute vec2 a_texCoord;   \n"
  "varying vec2 v_texCoord;\n"
  "void main() {\n"
  "   gl_Position = a_position;\n"
  "   v_texCoord = a_texCoord;\n"
  "}\n";
