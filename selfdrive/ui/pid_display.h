#define debug false

//void draw_pid_values(UIState *s, struct cereal_Live100Data datad) {
void draw_pid_values(UIState *s, 
  struct cereal_Live100Data datad, 
  int framenum,
  float *rolling_p,
  float *rolling_i,
  float *rolling_f
) {

  int numframes = 600;

  if (!s->vision_connected) {
    return;
  }

  int pos_x = 1920/2;
  int pos_y = 1080/2;
  int width = 750;
  int height = 500;
  nvgBeginPath(s->vg);
  nvgRoundedRect(s->vg, pos_x-(width/2), pos_y-(height/2), width, height, 100);
  nvgStrokeColor(s->vg, nvgRGBA(255,255,255,80));
  nvgStrokeWidth(s->vg, 6);
  nvgStroke(s->vg);
  nvgFillColor(s->vg, nvgRGBA(0, 0, 0, 100));
  nvgFill(s->vg);

  char content[500];
  float total = 0;
  float p_perc = 0;
  float i_perc = 0;
  float f_perc = 0;

  float avg_p = 0;
  float avg_i = 0;
  float avg_f = 0;

  float total_p = 0;
  float total_i = 0;
  float total_f = 0;

  int index = framenum % numframes;

  if (debug) {
    datad.upSteer = framenum; //debugger
  }

  rolling_p[index] = datad.upSteer;
  rolling_i[index] = datad.uiSteer;
  rolling_f[index] = datad.ufSteer;

  if (framenum >= numframes) {
    for (int i=0; i < numframes; i++) {
      total_p += rolling_p[i];
      total_i += rolling_i[i];
      total_f += rolling_f[i];
    }

    avg_p = total_p / numframes;
    avg_i = total_i / numframes;
    avg_f = total_f / numframes;

    /*
    avg_p = (numframes * avg_p + datad.upSteer) / (numframes+1);
    avg_i = (numframes * avg_i + datad.uiSteer) / (numframes+1);
    avg_f = (numframes * avg_f + datad.ufSteer) / (numframes+1);
    */

    if (debug) {
      printf("frame %02d: avg_p: %.6f (total: %f)\n",framenum%numframes,avg_p,total_p);
    }

    total = fabs(avg_p) + fabs(avg_i) + fabs(avg_f);

    p_perc = fabs(avg_p / total * 100);
    i_perc = fabs(avg_i / total * 100);
    f_perc = fabs(avg_f / total * 100);
  }

  snprintf(content,sizeof(content),"p: %.6f (%.1f%%)\ni: %.6f (%.1f%%)\nf: %.6f (%.1f%%)\nangleOffset: %.6f",avg_p,p_perc,avg_i,i_perc,avg_f,f_perc,datad.angleOffset);
  //snprintf(content,sizeof(content),"LaC.pid.p: 0.234\nLaC.pid.i: 0.123\nLac.pid.f: 0.456");

  nvgFillColor(s->vg, nvgRGBA(255, 255, 255, 200));
  nvgTextAlign(s->vg, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);
  nvgFontSize(s->vg, 55);
  nvgTextBox(s->vg,pos_x-(width/2)+40,pos_y-(height/2)+80,width+100,content,NULL);
  nvgRestore(s->vg);
}
