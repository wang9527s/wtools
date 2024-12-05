
#include "glwidget.h"
#include "ImagePathnameManager.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

#include <QTimer>
#include <QDebug>
#include <QDir>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    ,
    // clearColor(QColor(255, 0, 0, 60)),
    clearColor(Qt::transparent)
    , xRot(0)
    , yRot(0)
    , zRot(0)
    , program(0)
{
    // 背景透明 https://www.cnblogs.com/errorman/p/17217235.html
    setAttribute(Qt::WA_AlwaysStackOnTop);
    memset(textures, 0, sizeof(textures));

    QTimer *pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, [=] { rotateBy(+2 * 16, +2 * 16, -1 * 16); });
    pTimer->start(100);
}

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    for (int i = 0; i < 6; ++i)
        delete textures[i];
    delete program;
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(100, 100);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLWidget::updateTextures(QImage img)
{
    makeCurrent();
    QImage image = img.scaled(300, 300, Qt::KeepAspectRatio);
    for (int i = 0; i < 6; i++) {
        if (textures[i]) {
            textures[i]->destroy();
            delete textures[i];
        }
        textures[i] = new QOpenGLTexture(img.mirrored());
    }
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);                                // 启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置混合函数

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char *vsrc = "attribute highp vec4 vertex;\n"
                       "attribute mediump vec4 texCoord;\n"
                       "varying mediump vec4 texc;\n"
                       "uniform mediump mat4 matrix;\n"
                       "void main(void)\n"
                       "{\n"
                       "    gl_Position = matrix * vertex;\n"
                       "    texc = texCoord;\n"
                       "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc = "uniform sampler2D texture;\n"
                       "varying mediump vec4 texc;\n"
                       "void main(void)\n"
                       "{\n"
                       "    gl_FragColor = texture2D(texture, texc.st);\n"
                       "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("texture", 0);
}

void GLWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.ortho(-0.4f, +0.4f, +0.4f, -0.4f, 4.0f, 15.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("matrix", m);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(
        PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    for (int i = 0; i < 6; ++i) {
        textures[i]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    qInfo() << width << height;
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    return QOpenGLWidget::mousePressEvent(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    }
    else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();

    return QOpenGLWidget::mouseMoveEvent(event);
}

void GLWidget::makeObject()
{
    static const int coords[6][4][3] = {{{+1, -1, -1}, {-1, -1, -1}, {-1, +1, -1}, {+1, +1, -1}},
                                        {{+1, +1, -1}, {-1, +1, -1}, {-1, +1, +1}, {+1, +1, +1}},
                                        {{+1, -1, +1}, {+1, -1, -1}, {+1, +1, -1}, {+1, +1, +1}},
                                        {{-1, -1, -1}, {-1, -1, +1}, {-1, +1, +1}, {-1, +1, -1}},
                                        {{+1, -1, +1}, {-1, -1, +1}, {-1, -1, -1}, {+1, -1, -1}},
                                        {{-1, -1, +1}, {+1, -1, +1}, {+1, +1, +1}, {-1, +1, +1}}};

    // 初始化，使用exe包含的资源图片
    QString bg = ImagePathnameManager::instance()->pathname(0);
    for (int j = 0; j < 6; ++j) {
        textures[j] = new QOpenGLTexture(QImage(bg).mirrored());
    }

    QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            vertData.append(0.2 * coords[i][j][0]);
            vertData.append(0.2 * coords[i][j][1]);
            vertData.append(0.2 * coords[i][j][2]);
            // texture coordinate
            vertData.append(j == 0 || j == 3);
            vertData.append(j == 0 || j == 1);
        }
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}
