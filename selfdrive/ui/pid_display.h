void draw_pid_values(UIState *s, struct cereal_Live100Data datad) {
//void draw_pid_values(UIState *s) {

  if (!s->vision_connected) {
    return;
  }

  int pos_x = 1920/2;
  int pos_y = 1080/2;
  int width = 650;
  int height = 500;
  nvgBeginPath(s->vg);
  nvgRoundedRect(s->vg, pos_x-(width/2), pos_y-(height/2), width, height, 100);
  nvgStrokeColor(s->vg, nvgRGBA(255,255,255,80));
  nvgStrokeWidth(s->vg, 6);
  nvgStroke(s->vg);
  nvgFillColor(s->vg, nvgRGBA(0, 0, 0, 100));
  nvgFill(s->vg);

  char content[256];
  float total = fabs(datad.upSteer) + fabs(datad.uiSteer) + fabs(datad.ufSteer);
  float p_perc = 0;
  float i_perc = 0;
  float f_perc = 0;

  if (total > 0) {
    p_perc = fabs(datad.upSteer) / total * 100;
    i_perc = fabs(datad.uiSteer) / total * 100;
    f_perc = fabs(datad.ufSteer) / total * 100;
  }

  snprintf(content,sizeof(content),"p: %.6f (%.1f%%)\ni: %.6f (%.1f%%)\nf: %.6f (%.1f%%)\n",datad.upSteer,p_perc,datad.uiSteer,i_perc,datad.ufSteer,f_perc);
  //snprintf(content,sizeof(content),"LaC.pid.p: 0.234\nLaC.pid.i: 0.123\nLac.pid.f: 0.456");

  nvgFillColor(s->vg, nvgRGBA(255, 255, 255, 200));
  nvgTextAlign(s->vg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);
  nvgFontSize(s->vg, 55);
  nvgTextBox(s->vg,pos_x-(width/2)+40,pos_y-(height/2)+80,width+100,content,NULL);
  nvgRestore(s->vg);
}
