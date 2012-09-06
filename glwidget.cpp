// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "glwidget.h"
#include <QtCore/QDebug>

GLWidget::GLWidget(QWidget* parent)
    : QGLWidget(parent)
    , texture_fbo(NULL)
    , render_fbo(NULL)
{
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);
    setAutoFillBackground(false);
    makeCurrent();
}


GLWidget::~GLWidget()
{
    delete texture_fbo;
    if (texture_fbo != render_fbo)
        delete render_fbo;
}


void GLWidget::initializeGL()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    // glEnable(GL_TEXTURE_2D);
    // glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);
    glMatrixMode(GL_PROJECTION);
    // gluOrtho2D(-1, 1, -1, 1);
    glLoadIdentity();
}


void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}


void GLWidget::paintGL(void)
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(-1.0f, -1.0f, 0);
    glScalef(2.0f, 2.0f, 1.0f);
    for (DNAType::const_iterator genome = mDNA.constBegin(); genome != mDNA.constEnd(); ++genome) {
        glBegin(GL_POLYGON);
        glColor4f(genome->color().redF(), genome->color().greenF(), genome->color().blueF(), genome->color().alphaF());
        for (QPolygonF::const_iterator p = genome->polygon().constBegin(); p != genome->polygon().constEnd(); ++p) {
            glVertex2f(p->x(), p->y());
        }
        glEnd();
    }
}


void GLWidget::setData(const DNA& dna, const QSize& size)
{
    mDNA = dna;
    mSize = size;
    setMinimumSize(mSize);

    if (texture_fbo != NULL && render_fbo != NULL) {
        delete texture_fbo;
        if (texture_fbo != render_fbo)
            delete render_fbo;
    }
    render_fbo = new QGLFramebufferObject(mSize);
    texture_fbo = render_fbo;

    // TODO: make array from polygons
    updateGL();
}


void GLWidget::setOriginalImage(const QImage& originalImage)
{
    mOriginalImage = originalImage;
    // TODO: set image as GL texture
}


void GLWidget::paintEvent(QPaintEvent* event)
{
#if 0
    QPainter p;
    p.begin(this);
    p.setPen(Qt::transparent);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(event->rect(), QBrush(Qt::white));
    p.setRenderHint(QPainter::Antialiasing);
    p.scale(mSize.width(), mSize.height());
    for (DNAType::const_iterator genome = mDNA.constBegin(); genome != mDNA.constEnd(); ++genome) {
        p.setBrush(genome->color());
        p.drawPolygon(genome->polygon());
    }
    p.end();
#else
    Q_UNUSED(event);
#endif
}
