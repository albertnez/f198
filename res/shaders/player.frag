uniform float time;

void main() {
  vec2 uv = gl_TexCoord[0].xy - vec2(0.5);

  float tf = 5.0;
  float d = pow((1.5 + 0.2*sin(time*tf))*distance(vec2(0,0), uv), 2.0);

  gl_FragColor = vec4(d, d, d, d);
}

