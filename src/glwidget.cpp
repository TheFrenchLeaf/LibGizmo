/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>
#include <iostream>

#include <math.h>

#include "glwidget.h"
#include "qtlogo.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    logo = NULL;
    
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    
    gizmoMove = CreateMoveGizmo();
    gizmoRotate = CreateRotateGizmo();
    gizmoScale = CreateScaleGizmo();

    //gizmo = gizmoRotate; gizmo->SetLocation( IGizmo::LOCATE_LOCAL );
    gizmo = gizmoMove; gizmo->SetLocation( IGizmo::LOCATE_WORLD);

    
    float objectMatrixT[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    memcpy(objectMatrix, objectMatrixT, 16*sizeof(float));
    gizmo->SetEditMatrix( objectMatrix );
    gizmo->SetScreenDimension( this->width(), this->height() );
    gizmoMove->SetDisplayScale( 2.f );
    gizmoRotate->SetDisplayScale( 2.f );
    gizmoScale->SetDisplayScale( 2.f );

    setMouseTracking(true);
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());

    logo = new QtLogo(this, 64);
    logo->setColor(qtGreen.dark());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glClearDepth(1.0f);									// Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glDepthMask(1);
}
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
        
    float viewMat[16];
	  float projMat[16];         
	  glGetFloatv (GL_MODELVIEW_MATRIX, viewMat );  
	  glGetFloatv (GL_PROJECTION_MATRIX, projMat );  
	  gizmo->SetCameraMatrix( viewMat, projMat );

    memcpy(viewMat, objectMatrix, 16*sizeof(float));

    // Draw class QTLogo object
    glPushMatrix();
    glMultMatrixf( objectMatrix );
    logo->draw();
    glPopMatrix();

    // Draw the gizmo object
    if (gizmo)
    {
      glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
      gizmo->Draw();
      glPopAttrib();
    }
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3,3,10,0,0,0,0,1,0);

    if (gizmo)
        gizmo->SetScreenDimension( width, height );
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (gizmo /*&& event->buttons() & Qt::LeftButton*/)
      gizmo->OnMouseDown( event->x(), event->y());

    this->update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
  std::cout << event->x() << " " << event->y() << std::endl;
  if (gizmo /*&& event->buttons() & Qt::LeftButton*/)
    //gizmo->OnMouseMove( event->x(), event->y() );
    gizmo->OnMouseMove( event->x(), event->y() );
    //gizmo->OnMouseMove(this->width()/2.0, this->height()/2.0);

  this->update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (gizmo /*&& event->buttons() & Qt::LeftButton*/)
    gizmo->OnMouseUp( event->x(), event->y() );

  this->update();
}
