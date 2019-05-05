#include "classes.h"

Global::Global() {
  xres = 1920;
  yres = 1080;
  //memset(keys, 0, 65536);
}


Base::Base() {
  VecZero(dir);
  VecZero(vec);
  VecZero(projection);
  angle[0] = 90;	// xy plane (360) x is right left, y is forward backwards
  angle[1] = 90;	// z angle (180) 0 = up, 180 = down
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;

  xyz[0] = 0.0;
  xyz[1] = 0.0;
  xyz[2] = 0.0;
  xyz2[0] = 0.0;
  xyz2[1] = 0.0;
  xyz2[2] = 0.0;
  polar[0] = 0.0;
  polar[1] = 0.0;
  polar[2] = 0.0;

  xScale = 0.0;
  yScale = 0.0;

  radius = 50;
  currentHealth = 3;
  maxHealth = 3;
}

void Base::updatePolar(Vec ship) {

  xyz[0] = pos[0] - ship[0];
  xyz2[0] = xyz[0]*xyz[0];

  xyz[1] = pos[1] - ship[1];
  xyz2[1] = xyz[1] * xyz[1];

  xyz[2] = pos[2] - ship[2];
  xyz2[2] = xyz[2]*xyz[2];

  polar[0] = sqrt(xyz2[0] + xyz2[1] + xyz2[2]);
  //polar[1] = acos(xyz[2]/(sqrt(xyz2[0] + xyz2[1] + xyz2[2])));
  polar[2] = acos(xyz[2]/polar[0]);
  polar[2] *= 180/PI;
  if (xyz[0]) {
    polar[1] = atan2(xyz[1],xyz[0]);
    polar[1] *= 180/PI;
  }
  else
    polar[1] = 90.0;
  if (polar[1] > 180) {
     polar[1] -= 180;
     polar[1] = 180 - polar[2];
  }
}

void Base::drawBase(Game * g, Global gl) {

  float e[3];

  e[0] = 0;
  e[1] = polar[1];
  e[2] = polar[2];

  if (e[1] < 0) {
    e[1] = 360 + e[1];
  }

  float s[2];
  s[0] = (*g).ship.angle[0];
  s[1] = (*g).ship.angle[1];
  float low, high;
  low = s[0] - 60;
  high = s[0] + 60;
  high -= low;
  e[1] -= low;

  if (e[1] > 360) {
    e[1] = e[1] - 360;
  }

  projection[0] = ((high - e[1])/120)*gl.xres;
  projection[1] = ((s[1] + 45 - e[2])/90)*gl.yres;
  float x = projection[0];
  float y = projection[1];
  float tempValue = 0;

  //Scale max at the right edge of the setup_screen

  xScale = ((high - e[1])/60);
  yScale = ((s[1] + 45 - e[2])/45);

  if (xScale  > 1.0) {
    tempValue = xScale - 1.0;
    xScale = 1.0;
    xScale = xScale - tempValue;
  }

  if (yScale  > 1.0) {
    tempValue = yScale - 1.0;
    yScale = 1.0;
    yScale = yScale - tempValue;
  }


  float distanceScale = 12/polar[0];

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_POLYGON);

  //Override to different Vertices for different classes?
  glVertex2i(x-(radius*xScale*distanceScale),y-(radius*yScale*distanceScale));
  glVertex2i(x-(radius*xScale*distanceScale),y+(radius*yScale*distanceScale));
  glVertex2i(x+(radius*xScale*distanceScale),y-(radius*yScale*distanceScale));
  glVertex2i(x+(radius*xScale*distanceScale),y+(radius*yScale*distanceScale));

  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);


  glEnd();


  float cx = gl.xres/2;
  //float cy = gl.yres/2;
  Rect r;
  //
  r.bot = y + 10;
  r.left = x;
  r.center = 0;
  //ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
  ggprint8b(&r, 16, 0x00ffff00, "%.1i",currentHealth);




}
void Base::drawBullet(Game * g, Global gl) {

  float e[3];
	e[0] = 0;
	e[1] = polar[1];
	e[2] = polar[2];

	 if (e[1] < 0) {
	      e[1] = 360 + e[1];
	  }

	float s[2];
	s[0] = (*g).ship.angle[0];
	s[1] = (*g).ship.angle[1];
	float low, high;
	low = s[0] - 60;
	high = s[0] + 60;
	high -= low;
	e[1] -= low;
	if (e[1] > 360) {
	    e[1] = e[1] - 360;
	}
	float x, y;

	x = ((high - e[1])/120)*gl.xres;
	y = ((s[1] + 45 - e[2])/90)*gl.yres;

  float distanceScale = 48/polar[0];

	//glColor3fv(color);
	glPushMatrix();
	glBegin(GL_POLYGON);

	glVertex2i(x-radius*distanceScale,y);
	glVertex2i(x,y+radius*distanceScale);
	glVertex2i(x+radius*distanceScale,y);
	glVertex2i(x,y-radius*distanceScale);

	glEnd();
	glPopMatrix();

}


Image::Image(const char *fname) {
        if (fname[0] == '\0')
                return;
        //printf("fname **%s**\n", fname);
        int ppmFlag = 0;
        char name[40];
        strcpy(name, fname);
        int slen = strlen(name);
        char ppmname[80];
        if (strncmp(name+(slen-4), ".ppm", 4) == 0)
                ppmFlag = 1;
        if (ppmFlag) {
                strcpy(ppmname, name);
        } else {
                name[slen-4] = '\0';
                //printf("name **%s**\n", name);
                sprintf(ppmname,"%s.ppm", name);
                //printf("ppmname **%s**\n", ppmname);
                char ts[100];
                //system("convert eball.jpg eball.ppm");
                sprintf(ts, "convert %s %s", fname, ppmname);
                system(ts);
        }
        //sprintf(ts, "%s", name);
        FILE *fpi = fopen(ppmname, "r");
        if (fpi) {
                char line[200];
                fgets(line, 200, fpi);
                fgets(line, 200, fpi);
                //skip comments and blank lines
                while (line[0] == '#' || strlen(line) < 2)
                        fgets(line, 200, fpi);
                sscanf(line, "%i %i", &width, &height);
                fgets(line, 200, fpi);
                //get pixel data
                int n = width * height * 3;
                data = new unsigned char[n];
                for (int i=0; i<n; i++)
                        data[i] = fgetc(fpi);
                fclose(fpi);
        } else {
        printf("ERROR opening image: %s\n",ppmname);
                exit(0);
        }
        if (!ppmFlag)
unlink(ppmname);
}

Ship::Ship(int x, int y, int z) {
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;

  color[0] = color[1] = color[2] = 1.0;
  color[3] = .1;
  invert = false;

  color[0] = color[1] = color[2] = 1.0;
  maxHealth = 5;
  currentHealth = maxHealth;
  powerLevel = 1;
  maxBullets = MAX;
}

Object::Object(int x, int y, int z) {
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
}

Bullet::Bullet()
{
  radius = 5;
}


Asteroid::Asteroid() {
		prev = NULL;
		next = NULL;
    shipClass  = rand()%5;
    maxHealth = 3;
    currentHealth = maxHealth;
}


Game::Game(int xWindowSize, int yWindowSize, const Ship& ship, const Object& object) : ship(ship), object(object)
 {

	show_credits = false;
	ahead = NULL;
	barr = new Bullet[MAX_BULLETS];
  earr = new Enemy[MAX_ENEMIES];
	nasteroids = 0;
  nenemies = 0;
	nbullets = 0;
	mouseThrustOn = false;
	mtext = 0;
  difficulty = 1.0;
  level = 1;



  num_stars = 32000;
  /*
	//build 1 asteroids...
	for (int j=0; j<1; j++) {
		Asteroid *a = new Asteroid;
		a->nverts = 8;
		a->radius = rnd()*80.0 + 40.0;
		Flt r2 = a->radius / 2.0;
		Flt angle = 0.0f;
		Flt inc = (PI * 2.0) / (Flt)a->nverts;
		for (int i=0; i<a->nverts; i++) {
			a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
			a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
			angle += inc;
		}
		a->pos[0] = (Flt)(rand() % xWindowSize);
		a->pos[1] = (Flt)(rand() % yWindowSize);
		a->pos[2] = 0.0f;
		a->angle = 0.0;
		a->rotate = rnd() * 4.0 - 2.0;
		a->color[0] = 0.8;
		a->color[1] = 0.8;
		a->color[2] = 0.7;
		a->vel[0] = (Flt)(rnd()*2.0-1.0);
		a->vel[1] = (Flt)(rnd()*2.0-1.0);
		//std::cout << "asteroid" << std::endl;
		//add to front of linked list
		a->next = ahead;
		if (ahead != NULL)
			ahead->prev = a;
		ahead = a;
		++nasteroids;
	}
  */
	clock_gettime(CLOCK_REALTIME, &bulletTimer);
}