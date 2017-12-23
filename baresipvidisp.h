#ifndef BARESIPVIDISP_H
#define BARESIPVIDISP_H

#include <QQuickItem>


#include "baresipcore.h"

#include <re.h>
#include <rem.h>
#include <baresip.h>

class YUVTextureMaterial;

class QSGGeometry;
class QOpenGLContext;
class QOpenGLTexture;
class QOffscreenSurface;

class BaresipVidisp : public QQuickItem {
    Q_OBJECT
  
    YUVTextureMaterial* m_material;
    QSGGeometry* m_geometry;

    QOffscreenSurface* m_surface;
    QOpenGLContext* m_context;

    QOpenGLTexture* m_ytex;
    QOpenGLTexture* m_utex;
    QOpenGLTexture* m_vtex;

    void createTextures(int w, int h);
    void uploadTexture(const vidframe* vf);

public:

public:
    //friend class BaresipThread;
    BaresipVidisp();
    virtual ~BaresipVidisp();

signals:
	//void presentFrame(const struct vidframe*);

protected:

    QSGNode* updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);

    
private:
    struct _vidisp_st {
        const struct vidisp* vd;
        BaresipVidisp* vidisp;
    };

public:
	static void register_vidisp();
private:
    static int alloc(struct vidisp_st **vp,
            const struct vidisp *vd, struct vidisp_prm *prm,
            const char *dev,
            vidisp_resize_h *resizeh, void *arg);
    static int display(struct vidisp_st *st, const char *title,
                               const struct vidframe *bsframe);
    static void destroy(void* arg);
};

#endif // BARESIPVIDISP_H
