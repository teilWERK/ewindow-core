#ifndef YUVTEXTUREMATERIAL_H
#define YUVTEXTUREMATERIAL_H

#include <QSGMaterial>

class QSGTexture;
class QOpenGLTexture;

class YUVTextureMaterial : public QSGMaterial
{
public:
    YUVTextureMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader() const override;
    int compare(const QSGMaterial *other) const override;

    //void setYTexture(QSGTexture *texture);
    //QSGTexture *ytexture() const { return m_ytexture; }
    void setYTexture(QOpenGLTexture *texture);
    QOpenGLTexture *ytexture() const { return m_ytexture; }

    void setUTexture(QOpenGLTexture *texture);
    QOpenGLTexture *utexture() const { return m_utexture; }

    void setVTexture(QOpenGLTexture *texture);
    QOpenGLTexture *vtexture() const { return m_vtexture; }

protected:
    QOpenGLTexture *m_ytexture;
    QOpenGLTexture *m_utexture;
    QOpenGLTexture *m_vtexture;
};

class YUVTextureMaterialShader : public QSGMaterialShader
{
public:
    YUVTextureMaterialShader();

    void updateState(const RenderState& state, QSGMaterial* newEffect, QSGMaterial* oldEffect) override;
    char const* const* attributeNames() const override;

    static QSGMaterialType type;

protected:
    void initialize() override;
    void deactivate() override;

    const char* fragmentShader() const override;
    const char* vertexShader() const override;

    int m_matrix_id;
};

#endif // YUVTEXTUREMATERIAL_H
