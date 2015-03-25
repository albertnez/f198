uniform float time;
uniform vec2 player_pos;

void main() {
  const float kx = 1.0;
  const float ky = 1.0;
  const float kt = 4.0;

  float x = gl_FragCoord.x;
  float y = gl_FragCoord.y;

  float d = distance(gl_FragCoord.xy, player_pos);
  float r = clamp(10.0/d, 0.0, 1.0);

  float dx = abs(player_pos.x - x);

  float dy = abs(player_pos.y - y);

  float g = 0.0f;
  
  float kd = 50.0;
  float k_num_points = 7.00;
  float dist_factor = clamp(kd/d, 0.2, kd);
  float b = clamp((sin(x/k_num_points) * sin(y/k_num_points))*dist_factor, 0.0, 1.0);
  b *= 1.0 + (abs(sin(time*kt)));
  gl_FragColor = vec4(0.3*r, 0.3*r,0.3*r + b, 1);
}

