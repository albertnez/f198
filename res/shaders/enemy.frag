uniform float time;

void main() {
  vec2 uv = gl_TexCoord[0].xy - vec2(0.5);

  float sd = 0.08;
  float pf = 10.0;
  float tf = 10.0f;
  vec2 nuv = gl_TexCoord[0].xy;
  float auxy = nuv.y;
  nuv.y += sin(nuv.x*pf + time*tf)*sd;
  nuv.x += sin(auxy*pf + time*tf)*sd;
    
  uv.x += sin(uv.y*60.0)*0.05;
  uv.y += sin(uv.x*60.0)*0.05;
  
  float d = 0.05;
  float b = 0.0; // border

  // Y axis borders
  float dx = min(abs(1-d-nuv.x), abs(d - nuv.x));
  if (dx < d) {
    b += 1 - dx;
  }
  
  // X axis borders
  float dy = min(abs(1-d-nuv.y), abs(d - nuv.y));
  if (dy < d) {
    b += 1 - dx;
  }

  float l = length(uv);

  float m = 1.5+sin(pow(0.5-l,2.0)*20.0+time*4.0)*0.5;
  
  float c = (0.5-l)*m;
  gl_FragColor = vec4(b+c, b, b, b+c);
}

