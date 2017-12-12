#include "yuvtexturematerial.h"

#include <QSGTexture>
#include <QOpenGLTexture>

QSGMaterialType YUVTextureMaterialShader::type;

YUVTextureMaterialShader::YUVTextureMaterialShader() :
    QSGMaterialShader()
{
}

char const *const *YUVTextureMaterialShader::attributeNames() const
{
    static char const *const attr[] = { "qt_VertexPosition", "qt_VertexTexCoord", 0 };
    return attr;
}

void YUVTextureMaterialShader::initialize()
{
    m_matrix_id = program()->uniformLocation("qt_Matrix");
}

const char* YUVTextureMaterialShader::vertexShader() const {
    return
        "uniform highp mat4 qt_Matrix;"
        "attribute highp vec4 qt_VertexPosition;"
        "attribute highp vec2 qt_VertexTexCoord;"

        "varying highp vec2 qt_TexCoord;"

        "void main()"
        "{"
        "    qt_TexCoord = qt_VertexTexCoord;"
        "    gl_Position = qt_Matrix * qt_VertexPosition;"
        "}";
}

const char* YUVTextureMaterialShader::fragmentShader() const {
    return
        "uniform sampler2D Ytex;"
        "uniform sampler2D Utex,Vtex;"
        "varying highp vec2 qt_TexCoord;"
        "void main(void) {"
        "    lowp float r,g,b,y,u,v;"
        "    y=texture2D(Ytex,qt_TexCoord).r;"
        "    u=texture2D(Utex,qt_TexCoord).r; "
        "    v=texture2D(Vtex,qt_TexCoord).r; "
        "    y=1.1643*(y-0.0625);"
        "    u=u-0.5;"
        "    v=v-0.5;"
        ""
        "    r=y+1.5958*v;"
        "    g=y-0.39173*u-0.81290*v;"
        "    b=y+2.017*u; "
        "    gl_FragColor=vec4(r,g,b,1.0);"
        "}";
}

void YUVTextureMaterialShader::updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect)
{
    //qInfo() << "Shader update state ??????????????????????";
    Q_ASSERT(oldEffect == 0 || newEffect->type() == oldEffect->type());
    YUVTextureMaterial *tx = static_cast<YUVTextureMaterial *>(newEffect);
    YUVTextureMaterial *oldTx = static_cast<YUVTextureMaterial *>(oldEffect);

    QOpenGLTexture *y = tx->ytexture();
    QOpenGLTexture *u = tx->utexture();
    QOpenGLTexture *v = tx->vtexture();

    if (y)
        tx->ytexture()->bind(0);
    if (u)
        tx->utexture()->bind(1);
    if (v)
        tx->vtexture()->bind(2);

    if (state.isMatrixDirty())
        program()->setUniformValue(m_matrix_id, state.combinedMatrix());

    program()->setUniformValue("Ytex", 0);
    program()->setUniformValue("Utex", 1);
    program()->setUniformValue("Vtex", 2);
}

void YUVTextureMaterialShader::deactivate()
{
    //qInfo() << "unbind";
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

}

YUVTextureMaterial::YUVTextureMaterial()
    : QSGMaterial()
{
    m_ytexture = 0;
    m_utexture = 0;
    m_vtexture = 0;
}

QSGMaterialType *YUVTextureMaterial::type() const
{
    return &YUVTextureMaterialShader::type;
}

QSGMaterialShader *YUVTextureMaterial::createShader() const
{
    return new YUVTextureMaterialShader;
}

void YUVTextureMaterial::setYTexture(QOpenGLTexture *texture)
{
    m_ytexture = texture;
}

void YUVTextureMaterial::setUTexture(QOpenGLTexture *texture)
{
    m_utexture = texture;
}

void YUVTextureMaterial::setVTexture(QOpenGLTexture *texture)
{
    m_vtexture = texture;
}

int YUVTextureMaterial::compare(const QSGMaterial *o) const
{
    qInfo("texmaterial compare");
    Q_ASSERT(o && type() == o->type());
    const YUVTextureMaterial *other = static_cast<const YUVTextureMaterial *>(o);

    if (!m_ytexture || !other->ytexture()) return -1;
    // HACK: This is no proper compare, color channels are ignored
    int diff = m_ytexture->textureId() - other->ytexture()->textureId();

    qInfo() << "texmaterial compare" << diff;
    return diff;
}
