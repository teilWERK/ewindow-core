#ifndef BARESIPVIDISP_H
#define BARESIPVIDISP_H

#include <QDebug>

#include <QQuickItem>
#include <QQuickWindow>
#include <QCoreApplication>

#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QOffscreenSurface>

#include "yuvtexturematerial.h"

#include "baresipcore.h"

#include <re.h>
#include <rem.h>
#include <baresip.h>

class QAbstractVideoSurface;

class BaresipVidisp : public QQuickItem {
    Q_OBJECT

    QOffscreenSurface* m_surface;
    QOpenGLContext* m_context;

    QOpenGLTexture* m_ytex;
    QOpenGLTexture* m_utex;
    QOpenGLTexture* m_vtex;

public:
    void createTextures(int w, int  h) {
        if (m_ytex) return;

        qInfo() << "creating textures...";

        QOpenGLTexture::TextureFormat format = QOpenGLTexture::R8_UNorm;

        qInfo() << "isgles" << m_context->isOpenGLES();
        if (m_context->isOpenGLES())
            format = QOpenGLTexture::LuminanceFormat;
        QOpenGLTexture::PixelFormat pformat = QOpenGLTexture::Luminance;
        QOpenGLTexture::PixelType ptype = QOpenGLTexture::UInt8;

        QOpenGLTexture::Target target = QOpenGLTexture::Target2D;

        //QImage img = QImage("test.png");
        //m_ytex = new QOpenGLTexture(img);

        m_ytex = new QOpenGLTexture(target);
        m_ytex->setFormat(format);
        m_ytex->setSize(w, h);
        m_ytex->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        m_ytex->allocateStorage(pformat, ptype);

        m_utex = new QOpenGLTexture(target);
        m_utex->setFormat(format);
        m_utex->setSize(w / 2, h / 2);
        m_utex->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        m_utex->allocateStorage(pformat, ptype);

        m_vtex = new QOpenGLTexture(target);
        m_vtex->setFormat(format);
        m_vtex->setSize(w / 2, h / 2);
        m_vtex->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        m_vtex->allocateStorage(pformat, ptype);

        m_material->setYTexture(m_ytex);
        m_material->setUTexture(m_utex);
        m_material->setVTexture(m_vtex);
    }

public:
    friend class BaresipThread;
    BaresipVidisp()
    {
        qInfo("BaresipVidisp constructor");
        setFlag(ItemHasContents);

        m_context = 0;
        m_surface = 0;
        m_ytex = 0;
        m_utex = 0;
        m_vtex = 0;

        m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
        m_material = new YUVTextureMaterial;
    }

    ~BaresipVidisp() {
        qWarning() << "BaresipVidisp destroyed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

        delete m_geometry;
        delete m_material;

        delete m_ytex;
        delete m_utex;
        delete m_vtex;

        delete m_context;
        delete m_surface;
    }

signals:
	void presentFrame(const struct vidframe*);

protected:
    YUVTextureMaterial* m_material;
    QSGGeometry* m_geometry;

    QSGNode* updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData)
    {
        //qInfo() << "updatePaintNode" << boundingRect();
        if (!m_surface) {
            m_surface = new QOffscreenSurface();
            m_surface->setFormat(window()->format());
            m_surface->create();
            qInfo() << "create offscreen surface: " << window()->format() << m_surface->isValid();
        }

        if (!m_context) {
            //QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);
            qInfo() << "creating new context";
            QOpenGLContext* current = window()->openglContext();
            qInfo() << current->isValid();
            qInfo() << "unbind old context" << current->makeCurrent(0);
            current->doneCurrent();

            m_context = new QOpenGLContext();
            m_context->setFormat(current->format());
            m_context->setShareContext(current);
            bool ret = m_context->create();
            qInfo() << "context created: " << m_context << ret;
            m_context->doneCurrent();
            qInfo() << "make current " << m_context << window();
            ret = current->makeCurrent(window());

            qInfo() << "re-bind successful: " << ret;
            if (!ret) exit(1);

            QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);
        }

        QSGGeometryNode* n = static_cast<QSGGeometryNode*>(oldNode);
        if (!n) {
            n = new QSGGeometryNode();
            n->setMaterial(m_material);
            n->setGeometry(m_geometry);
        }

        QSGGeometry::updateTexturedRectGeometry(m_geometry, boundingRect(), QRectF(0, 0, 1, 1));

        //n->markDirty(QSGNode::DirtyMaterial);
        update();

        return n;
    }


    void uploadTexture(const vidframe* vf) {
        if (!m_context || !m_surface) {
            qInfo() << this << "no context yet, returning...";
            return;
        }

        m_context->makeCurrent(m_surface);

        if (!m_ytex) createTextures(vf->size.w, vf->size.h);

        //qInfo() << "uploading texture";
        m_ytex->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, (const void*)vf->data[0]);
        m_utex->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, (const void*)vf->data[1]);
        m_vtex->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, (const void*)vf->data[2]);

        // TODO: This is ineffective
        //update();
    }

private:
    struct _vidisp_st {
        const struct vidisp* vd;
        BaresipVidisp* vidisp;
    };

public:
    static int alloc(struct vidisp_st **vp,
            const struct vidisp *vd, struct vidisp_prm *prm,
            const char *dev,
            vidisp_resize_h *resizeh, void *arg)
    {
        qInfo() << "vidisp_alloc: ";

        _vidisp_st* st = (_vidisp_st*) mem_zalloc(sizeof(_vidisp_st), destroy);
        *vp = (vidisp_st*)st;

        st->vd = vd;
        st->vidisp = new BaresipVidisp();

        emit BaresipCore::instance().newVideo(st->vidisp);
        return 0;
    }

    static int display(struct vidisp_st *st, const char *title,
                               const struct vidframe *bsframe)
    {
        //qInfo() << "display" << title << bsframe->linesize[0];
        _vidisp_st* vst = (_vidisp_st*) st;
        vst->vidisp->uploadTexture(bsframe);

    }

    static void destroy(void* arg) {
        struct _vidisp_st* st = (_vidisp_st*) arg;
        delete st->vidisp;
    }
};

#endif // BARESIPVIDISP_H
