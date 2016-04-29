#include "PicoRenderer.h"

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void drawLineCube()
{
	
	glBegin(GL_LINES);
	glColor3f(1.0,1.0,1.0);

	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.0,0.0);

	glVertex3f(1.0,0.0,0.0);
	glVertex3f(1.0,1.0,0.0);

	glVertex3f(1.0,1.0,0.0);
	glVertex3f(0.0,1.0,0.0);

	glVertex3f(0.0,1.0,0.0);
	glVertex3f(0.0,0.0,0.0);

	glVertex3f(0.0,0.0,1.0);
	glVertex3f(1.0,0.0,1.0);

	glVertex3f(1.0,0.0,1.0);
	glVertex3f(1.0,1.0,1.0);

	glVertex3f(1.0,1.0,1.0);
	glVertex3f(0.0,1.0,1.0);

	glVertex3f(0.0,1.0,1.0);
	glVertex3f(0.0,0.0,1.0);

	glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,1.0);

	glVertex3f(1.0,0.0,0.0);
	glVertex3f(1.0,0.0,1.0);

	glVertex3f(1.0,1.0,0.0);
	glVertex3f(1.0,1.0,1.0);

	glVertex3f(0.0,1.0,0.0);
	glVertex3f(0.0,1.0,1.0);

	glEnd();
}

void drawStaticImage(PicoVec4f* RGBAinfo,int nx, int ny)
{
	glBegin(GL_QUADS);
	for(int i = 0; i < nx; ++i)
	{
		for(int j = 0; j < ny; ++j)
		{
			glColor3f(RGBAinfo[j*nx+i].x,
					  RGBAinfo[j*nx+i].y,
					  RGBAinfo[j*nx+i].z);

			glVertex3f(1.0*i/nx,1.0*j/ny,0.0);
			glVertex3f(1.0*(i+1)/nx,1.0*j/ny,0.0);
			glVertex3f(1.0*(i+1)/nx,1.0*(j+1)/ny,0.0);
			glVertex3f(1.0*i/nx,1.0*(j+1)/ny,0.0);
		}
	}
	glEnd();
}

void drawMineCraftScreen(VisualSystem* vsin)
{
	glColor3f(0.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,-0.01);
	glVertex3f(1.0,0.0,-0.01);
	glVertex3f(1.0,1.0,-0.01);
	glVertex3f(0.0,1.0,-0.01);
	glEnd();

	glPushMatrix();
	glTranslatef(0.02,0.02,0.0);
	glScalef(0.94,0.94,1.0);
	drawStaticImage(vsin->getVision(),
					vsin->getResX(),
					vsin->getResY());
	glPopMatrix();
}

void drawMineCraftAudioStream(AudioSystem * as)
{
	int nsamp = as->getNumberOfFreq();
	float * dataL = as->getLeftSpectrum();
	float * dataR = as->getRightSpectrum();
	float scale = 0.05, soundLevel1,soundLevel2;

	glColor3f(0.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,-0.01);
	glVertex3f(1.0,0.0,-0.01);
	glVertex3f(1.0,1.0,-0.01);
	glVertex3f(0.0,1.0,-0.01);
	glEnd();

	glPushMatrix();
	glTranslatef(0.02,0.02,0.0);
	glScalef(0.96,0.96,1.0);

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.0,0.0);
	glVertex3f(1.0,1.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	glEnd();

	glColor3f(0.0,1.0,1.0);
	glBegin(GL_LINES);
	for(int i = 1; i < nsamp-1; ++i)
	{
		soundLevel1 = scale*(dataL[i]); if(soundLevel1 > 0.5){soundLevel1 = 0.5;}
		soundLevel2 = scale*(dataL[i+1]); if(soundLevel2 > 0.5){soundLevel2 = 0.5;}
		glVertex3f(1.0*i/nsamp,0.5 + soundLevel1,0.01);
		glVertex3f(1.0*(i+1)/nsamp,0.5 + soundLevel2,0.01);

		soundLevel1 = scale*(dataR[i]); if(soundLevel1 > 0.5){soundLevel1 = 0.5;}
		soundLevel2 = scale*(dataR[i+1]); if(soundLevel2 > 0.5){soundLevel2 = 0.5;}
		glVertex3f(1.0*i/nsamp,soundLevel1,0.01);
		glVertex3f(1.0*(i+1)/nsamp,soundLevel2,0.01);
	}
	glEnd();
	glPopMatrix();
}

void renderBrainBox(BrainBox * bb, bool showSynapses)
{
	Neuron * sel1, * sel2;
	Synapse * synsel;
}

void renderSpike(Neuron * n)
{
	int ntail;
	
	if(n)
	{
		glPointSize(6.0);
		if(n->pos.y > 0.98){glColor3f(1.0,0.0,0.7);}
		else if(n->pos.y > 0.94){glColor3f(0.0,1.0,0.7);}
		else if(n->pos.y > 0.9){glColor3f(0.0,0.7,1.0);}
		else if(n->type == EXC){glColor3f(0.0,1.0,1.0);}
		else if(n->type == INH){glColor3f(1.0,0.0,1.0);}
		else{glColor3f(0.5,0.5,0.5);}
		glBegin(GL_POINTS);
		glVertex3f(n->pos.x,n->pos.y,n->pos.z);
		glEnd();
	}
}

void renderSpikes(BrainBox * bb, int showTail)
{
	std::queue<Neuron *> * q = &(bb->renderQueue);
	while(!q->empty())
	{
		renderSpike(q->front());
		q->pop();
	}
	drawLineCube();
}

void renderSelNeuron(Neuron * n, CircularRegister * pot, CircularRegister * thresh)
{
	int nSyn;
	float w,p1,p2;
	float synWidth = 0.05;
	float scale = 0.5;
	Neuron * neur;
	std::list<Synapse *>::iterator selSyn;

	glPushMatrix();
	glTranslatef(0.3,0.2,0.0);
	glScalef(0.4,0.6,1.0);

	if(n->type == EXC){glColor3f(0.0,1.0,1.0);}
	else if(n->type == INH){glColor3f(1.0,0.0,1.0);}
	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,-0.01);
	glVertex3f(1.0,0.0,-0.01);
	glVertex3f(1.0,1.0,-0.01);
	glVertex3f(0.0,1.0,-0.01);
	glEnd();

	glPushMatrix();
	glTranslatef(0.02,0.02,0.0);
	glScalef(0.96,0.96,1.0);

	glColor3f(0.0,0.0,0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0,0.0,0.0);
	glVertex3f(1.0,0.0,0.0);
	glVertex3f(1.0,1.0,0.0);
	glVertex3f(0.0,1.0,0.0);
	glEnd();

	glBegin(GL_LINES);
	for(int i = 1; i < CIRCREGLEN; ++i)
	{
		glColor3f(0.0,1.0,1.0);
		p1 = scale*(pot->read(i)); if(p1 > 0.9){p1 = 0.9;}
		p2 = scale*(pot->read(i+1)); if(p2 > 0.9){p2 = 0.9;}
		glVertex3f(1.0*i/CIRCREGLEN,0.1 + p1,0.01);
		glVertex3f(1.0*(i+1)/CIRCREGLEN,0.1 + p2,0.01);

		glColor3f(1.0,0.0,1.0);
		p1 = scale*(thresh->read(i)); if(p1 > 0.9){p1 = 0.9;}
		p2 = scale*(thresh->read(i+1)); if(p2 > 0.9){p2 = 0.9;}
		glVertex3f(1.0*i/CIRCREGLEN,0.1 + p1,0.01);
		glVertex3f(1.0*(i+1)/CIRCREGLEN,0.1 + p2,0.01);
	}
	glEnd();
	glPopMatrix();

	glPopMatrix();

	glBegin(GL_QUADS);
	nSyn = n->afferent.size();
	selSyn = n->afferent.begin();
	for(int i = 0; i < nSyn; ++i)
	{
		neur = (*selSyn)->preSynapticNeuron;
		w = (*selSyn)->weight;
		if(neur->type == EXC){glColor3f(0.0,w,w);}
		else if(neur->type == INH){glColor3f(w,0.0,w);}

		glVertex3f(0.0,1.0*i/nSyn,0.0);
		glVertex3f(synWidth,1.0*i/nSyn,0.0);
		glVertex3f(synWidth,1.0*(i+1)/nSyn,0.0);
		glVertex3f(0.0,1.0*(i+1)/nSyn,0.0);
		selSyn++;
	}
	glEnd();

	glColor3f(0.0,0.5,0.5);
	glBegin(GL_LINES);
	for(int i = 0; i < nSyn; ++i)
	{
		glVertex3f(synWidth,1.0*(i+0.5)/nSyn,-0.02);
		glVertex3f(0.5,0.5,-0.02);
	}
	glColor3f(0.0,1.0,1.0);
	glVertex3f(0.0,0.0,0.01);
	glVertex3f(synWidth,0.0,0.01);
	glVertex3f(0.0,0.0,0.01);
	glVertex3f(0.0,1.0,0.01);
	glVertex3f(synWidth,0.0,0.01);
	glVertex3f(synWidth,1.0,0.01);
	glVertex3f(0.0,1.0,0.01);
	glVertex3f(synWidth,1.0,0.01);
	glEnd();

	glBegin(GL_QUADS);
	nSyn = n->efferent.size();
	selSyn = n->efferent.begin();
	for(int i = 0; i < nSyn; ++i)
	{
		neur = (*selSyn)->postSynapticNeuron;
		w = (*selSyn)->weight;
		if(n->type == EXC){glColor3f(0.0,w,w);}
		else if(n->type == INH){glColor3f(w,0.0,w);}

		glVertex3f(1.0-synWidth,1.0*i/nSyn,0.0);
		glVertex3f(1.0,1.0*i/nSyn,0.0);
		glVertex3f(1.0,1.0*(i+1)/nSyn,0.0);
		glVertex3f(1.0-synWidth,1.0*(i+1)/nSyn,0.0);
		selSyn++;
	}
	glEnd();

	glColor3f(0.0,0.5,0.5);
	glBegin(GL_LINES);
	for(int i = 0; i < nSyn; ++i)
	{
		glVertex3f(1.0-synWidth,1.0*(i+0.5)/nSyn,-0.02);
		glVertex3f(0.5,0.5,-0.02);
	}
	glColor3f(0.0,1.0,1.0);
	glVertex3f(1.0-synWidth,0.0,0.01);
	glVertex3f(1.0,0.0,0.01);
	glVertex3f(1.0-synWidth,0.0,0.01);
	glVertex3f(1.0-synWidth,1.0,0.01);
	glVertex3f(1.0,0.0,0.01);
	glVertex3f(1.0,1.0,0.01);
	glVertex3f(1.0-synWidth,1.0,0.01);
	glVertex3f(1.0,1.0,0.01);
	glEnd();
}

void renderOutputState(OutputSystem * os)
{
	Latch ** outgrid = os->getOutputState();
	
	glBegin(GL_QUADS);
	for(int x = 0; x < OUTPUT_GRID; ++x)
	{
		for(int y = 0; y < OUTPUT_GRID; ++y)
		{
			if(outgrid[x][y].state == OUTPUT_UP)
			{glColor3f(0.0,1.0,1.0);}
			if(outgrid[x][y].state == OUTPUT_DOWN)
			{glColor3f(0.0,0.0,0.0);}
			glVertex3f(1.0*x/OUTPUT_GRID,0.0,1.0*y/OUTPUT_GRID);
			glVertex3f(1.0*(x+1)/OUTPUT_GRID,0.0,1.0*y/OUTPUT_GRID);
			glVertex3f(1.0*(x+1)/OUTPUT_GRID,0.0,1.0*(y+1)/OUTPUT_GRID);
			glVertex3f(1.0*x/OUTPUT_GRID,0.0,1.0*(y+1)/OUTPUT_GRID);
		}
	}
	glEnd();

	glColor3f(0.0,1.0,1.0);
	glBegin(GL_LINES);
	for(int x = 0; x <= OUTPUT_GRID; ++x)
	{
		glVertex3f(1.0*x/OUTPUT_GRID,0.01,0.0);
		glVertex3f(1.0*x/OUTPUT_GRID,0.01,1.0);
	}
	for(int y = 0; y <= OUTPUT_GRID; ++y)
	{
		glVertex3f(0.0,0.02,1.0*y/OUTPUT_GRID);
		glVertex3f(1.0,0.02,1.0*y/OUTPUT_GRID);
	}
	glEnd();
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void PicoRenderer::initialize(HWND hwndin,int widthin,int heightin)
{
	int pixelFormat;
	
	width = widthin;
	height = heightin;
	aspectRatio = float(width)/height;
	_hDC = GetDC(hwndin);

	PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  /* size */
        1,                              /* version */
        PFD_SUPPORT_OPENGL |
        PFD_DRAW_TO_WINDOW |
        PFD_DOUBLEBUFFER,               /* support double-buffering */
        PFD_TYPE_RGBA,                  /* color type */
        16,                             /* prefered color depth */
        0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
        0,                              /* no alpha buffer */
        0,                              /* alpha bits (ignored) */
        0,                              /* no accumulation buffer */
        0, 0, 0, 0,                     /* accum bits (ignored) */
        16,                             /* depth buffer */
        0,                              /* no stencil buffer */
        0,                              /* no auxiliary buffers */
        PFD_MAIN_PLANE,                 /* main layer */
        0,                              /* reserved */
        0, 0, 0,                        /* no layer, visible, damage masks */
    };
    pixelFormat = ChoosePixelFormat(_hDC, &pfd);
	SetPixelFormat(_hDC, pixelFormat, &pfd);

	_hGLRC = wglCreateContext(_hDC);
	wglMakeCurrent(_hDC, _hGLRC);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, width, height);
}

void PicoRenderer::update(PicoSimulation* xsimulation)
{
	char buffer[256];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cameraView(xsimulation->getxCamera());
		
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glScalef(10.0,10.0,10.0);
	glTranslatef(-0.5,-0.5,-0.5);
	//renderBrainBox(xsimulation->getBrainBox(),0);
	renderSpikes(xsimulation->getBrainBox(),0);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(11.0,0.0,0.0);
	glScalef(10.0,10.0,1.0);
	glTranslatef(-0.5,-0.5,0.0);
	drawMineCraftScreen(xsimulation->getVisualSystem());
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-11.0,0.0,0.0);
	glScalef(10.0,10.0,1.0);
	glTranslatef(-0.5,-0.5,0.0);
	drawMineCraftAudioStream(xsimulation->getAudioSystem());
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,-5.1,0.0);
	glScalef(10.0,1.0,10.0);
	glTranslatef(-0.5,0.0,-0.5);
	renderOutputState(xsimulation->getOutputSystem());
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,-8.0,0.0);
	glScalef(20.0,5.0,1.0);
	glTranslatef(-0.5,-0.5,0.0);
	renderSelNeuron(xsimulation->getSelNeuron(),
		xsimulation->getSelNeuronPot(),
		xsimulation->getSelNeuronThresh());
	glPopMatrix();

	glPopMatrix();
	glFlush();
	SwapBuffers(_hDC);
}

void PicoRenderer::cameraView(PicoCamera* xcam)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(xcam->_FoV, aspectRatio, 0.1, 1000.0);

	glTranslatef(0.0,0.0,-xcam->_distance);
	glRotatef(xcam->_elevation,1.0,0.0,0.0);
	glRotatef(-xcam->_azimuth,0.0,1.0,0.0);
	glTranslatef(-xcam->_position.x,-xcam->_position.y,-xcam->_position.z);
}

void PicoRenderer::release()
{
}

