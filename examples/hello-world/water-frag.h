char fShaderStr[] =
  "precision mediump float;\n"
  "// textures\n"
  "uniform sampler2D u_waterMap;\n"
  "uniform sampler2D u_textureShine;\n"
  "uniform sampler2D u_textureFg;\n"
  "uniform sampler2D u_textureBg;\n"
  "// the texCoords passed in from the vertex shader.\n"
  "varying vec2 v_texCoord;\n"
  "uniform vec2 u_resolution;\n"
  "uniform vec2 u_parallax;\n"
  "uniform float u_parallaxFg;\n"
  "uniform float u_parallaxBg;\n"
  "uniform float u_textureRatio;\n"
  "uniform bool u_renderShine;\n"
  "uniform bool u_renderShadow;\n"
  "uniform float u_minRefraction;\n"
  "uniform float u_refractionDelta;\n"
  "uniform float u_brightness;\n"
  "uniform float u_alphaMultiply;\n"
  "uniform float u_alphaSubtract;\n"
  "// alpha-blends two colors\n"
  "vec4 blend(vec4 bg,vec4 fg){\n"
  "  vec3 bgm=bg.rgb*bg.a;\n"
  "  vec3 fgm=fg.rgb*fg.a;\n"
  "  float ia=1.0-fg.a;\n"
  "  float a=(fg.a + bg.a * ia);\n"
  "  vec3 rgb;\n"
  "  if(a!=0.0){\n"
  "    rgb=(fgm + bgm * ia) / a;\n"
  "  }else{\n"
  "    rgb=vec3(0.0,0.0,0.0);\n"
  "  }\n"
  "  return vec4(rgb,a);\n"
  "}\n"
  "vec2 pixel(){\n"
  "  return vec2(1.0,1.0)/u_resolution;\n"
  "}\n"
  "vec2 parallax(float v){\n"
  "  return u_parallax*pixel()*v;\n"
  "}\n"
  "vec2 texCoord(){\n"
  "  return vec2(gl_FragCoord.x, u_resolution.y-gl_FragCoord.y)/u_resolution;\n"
  "}\n"
  "// scales the bg up and proportionally to fill the container\n"
  "vec2 scaledTexCoord(){\n"
  "  float ratio=u_resolution.x/u_resolution.y;\n"
  "  vec2 scale=vec2(1.0,1.0);\n"
  "  vec2 offset=vec2(0.0,0.0);\n"
  "  float ratioDelta=ratio-u_textureRatio;\n"
  "  if(ratioDelta>=0.0){\n"
  "    scale.y=(1.0+ratioDelta);\n"
  "    offset.y=ratioDelta/2.0;\n"
  "  }else{\n"
  "    scale.x=(1.0-ratioDelta);\n"
  "    offset.x=-ratioDelta/2.0;\n"
  "  }\n"
  "  return (texCoord()+offset)/scale;\n"
  "}\n"
  "// get color from fg\n"
  "vec4 fgColor(float x, float y){\n"
  "  float p2=u_parallaxFg*2.0;\n"
  "  vec2 scale=vec2(\n"
  "    (u_resolution.x+p2)/u_resolution.x,\n"
  "    (u_resolution.y+p2)/u_resolution.y\n"
  "  );\n"
  "  vec2 scaledTexCoord=texCoord()/scale;\n"
  "  vec2 offset=vec2(\n"
  "    (1.0-(1.0/scale.x))/2.0,\n"
  "    (1.0-(1.0/scale.y))/2.0\n"
  "  );\n"
  "  return texture2D(u_waterMap,\n"
  "    (scaledTexCoord+offset)+(pixel()*vec2(x,y))+parallax(u_parallaxFg)\n"
  "  ).bgra;\n"
  "}\n"
  "void main() {\n"
  "  vec4 bg=texture2D(u_textureBg,scaledTexCoord()+parallax(u_parallaxBg)).bgra;\n"
  "  vec4 cur = fgColor(0.0,0.0);\n"
  "  float d=cur.b;\n"
  "  float x=cur.g;\n"
  "  float y=cur.r;\n"
  "  float a=clamp(cur.a*u_alphaMultiply-u_alphaSubtract, 0.0,1.0);\n"
  "  vec2 refraction = (vec2(x,y)-0.5)*2.0;\n"
  "  vec2 refractionParallax=parallax(u_parallaxBg-u_parallaxFg);\n"
  "  vec2 refractionPos = scaledTexCoord()\n"
  "    + (pixel()*refraction*(u_minRefraction+(d*u_refractionDelta)))\n"
  "    + refractionParallax;\n"
  "  vec4 tex=texture2D(u_textureFg,refractionPos).bgra;\n"
  "  if(u_renderShine){\n"
  "    float maxShine=490.0;\n"
  "    float minShine=maxShine*0.18;\n"
  "    vec2 shinePos=vec2(0.5,0.5) + ((1.0/512.0)*refraction)* -(minShine+((maxShine-minShine)*d));\n"
  "    vec4 shine=texture2D(u_textureShine,shinePos).bgra;\n"
  "    tex=blend(tex,shine);\n"
  "  }\n"
  "  vec4 fg=vec4(tex.rgb*u_brightness,a);\n"
  "  if(u_renderShadow){\n"
  "    float borderAlpha = fgColor(0.,0.-(d*6.0)).a;\n"
  "    borderAlpha=borderAlpha*u_alphaMultiply-(u_alphaSubtract+0.5);\n"
  "    borderAlpha=clamp(borderAlpha,0.,1.);\n"
  "    borderAlpha*=0.2;\n"
  "    vec4 border=vec4(0.,0.,0.,borderAlpha);\n"
  "    fg=blend(border,fg);\n"
  "  }\n"
  "  gl_FragColor = blend(bg,fg);\n"
  "}\n";
