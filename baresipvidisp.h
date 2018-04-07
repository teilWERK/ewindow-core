#ifndef BARESIPVIDISP_H
#define BARESIPVIDISP_H

#include "api/mediastreaminterface.h"
#include "api/video/video_frame.h"

#include <QQuickItem>

class YUVTextureMaterial;

class QSGGeometry;
class QOpenGLContext;
class QOpenGLTexture;
class QOffscreenSurface;

class BaresipVidisp : public QQuickItem,
    public rtc::VideoSinkInterface<webrtc::VideoFrame>
    {
    Q_OBJECT
  
    YUVTextureMaterial* m_material;
    QSGGeometry* m_geometry;

    QOffscreenSurface* m_surface;
    QOpenGLContext* m_context;

    QOpenGLTexture* m_ytex;
    QOpenGLTexture* m_utex;
    QOpenGLTexture* m_vtex;

    void createTextures(int w, int h);
    //void uploadTexture();

    // VideoSinkInterface implementation
    void OnFrame(const webrtc::VideoFrame& frame) override;

public:
    BaresipVidisp(webrtc::VideoTrackInterface* track_to_render);
    virtual ~BaresipVidisp();

signals:
	//void presentFrame(const struct vidframe*);

protected:

    QSGNode* updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);
};

#endif // BARESIPVIDISP_H
