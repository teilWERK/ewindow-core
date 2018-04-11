#include "baresipvidisp.h"

#include "yuvtexturematerial.h"

#include <QCoreApplication>
#include <QDebug>

#include <QQuickWindow>
#include <QSGGeometryNode>

#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QOffscreenSurface>

BaresipVidisp::BaresipVidisp(webrtc::VideoTrackInterface* track_to_render)
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

	track_to_render->AddOrUpdateSink(this, rtc::VideoSinkWants());
}

BaresipVidisp::~BaresipVidisp()
{
	qInfo() << "\e[35mBaresipVidisp::~BaresipVidisp()\e[m";

	// Ownership moved to QSGGeometryNode
	//delete m_geometry;
	//delete m_material;

	m_material->setYTexture(0);
	m_material->setUTexture(0);
	m_material->setVTexture(0);

	// TODO: check for possible race condition with ::display?
	if (m_context) {
		m_context->makeCurrent(m_surface);
	}

	delete m_ytex;
	delete m_utex;
	delete m_vtex;

	delete m_context;
	delete m_surface;
}

void BaresipVidisp::createTextures(int w, int  h)
{
	if (m_ytex) return;

	qInfo() << "creating textures...";

	// Workaround for GLES: Desktop GL has R8, GLES2 only Luminance
	QOpenGLTexture::TextureFormat format = QOpenGLTexture::R8_UNorm;
	if (m_context->isOpenGLES())
		format = QOpenGLTexture::LuminanceFormat;

	QOpenGLTexture::PixelFormat pformat = QOpenGLTexture::Luminance;
	QOpenGLTexture::PixelType ptype = QOpenGLTexture::UInt8;

	QOpenGLTexture::Target target = QOpenGLTexture::Target2D;

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


QSGNode* BaresipVidisp::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData*)
{
	if (!m_surface) {
		m_surface = new QOffscreenSurface();
		m_surface->setFormat(window()->format());
		m_surface->create();
		qInfo() << "create offscreen surface: " << window()->format() << m_surface->isValid();
	}

	if (!m_context) {
//		QCoreApplication::setAttribute(Qt::AA_DontCheckOpenGLContextThreadAffinity);
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
	}

	QSGGeometryNode* n = static_cast<QSGGeometryNode*>(oldNode);
	if (!n) {
		n = new QSGGeometryNode();
		n->setMaterial(m_material);
		n->setGeometry(m_geometry);
		n->setFlag(QSGNode::OwnsGeometry);
		n->setFlag(QSGNode::OwnsMaterial);
		QSGGeometry::updateTexturedRectGeometry(m_geometry, boundingRect(), QRectF(0, 0, 1, 1));
	}

	n->markDirty(QSGNode::DirtyMaterial);
	n->markDirty(QSGNode::DirtyGeometry);
	update();

	return n;
}

//void BaresipVidisp::uploadTexture(const vidframe* vf) {
void BaresipVidisp::OnFrame(const webrtc::VideoFrame& frame) {
	if (!m_context || !m_surface) {
		qInfo() << this << "no context yet, returning...";
		return;
	}

	m_context->makeCurrent(m_surface);


	rtc::scoped_refptr<webrtc::I420BufferInterface> buffer(
		frame.video_frame_buffer()->ToI420());
	if (!m_ytex) createTextures(buffer->StrideY(), buffer->height());

	//qInfo() << "uploading texture";
	m_ytex->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, buffer->DataY());
	m_utex->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, buffer->DataU());
	m_vtex->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, buffer->DataV());

	float x_scale = float(buffer->width()) / float(buffer->StrideY());
	QSGGeometry::updateTexturedRectGeometry(m_geometry, boundingRect(), QRectF(0, 0, x_scale, 1));

	m_context->doneCurrent();
}

/*
void BaresipVidisp::register_vidisp() {
	struct vidisp* st;
	int ret = vidisp_register(&st, baresip_vidispl(), "qtupload",
					BaresipVidisp::alloc, 0, BaresipVidisp::display, 0);
	qInfo() << "vidisp_register " << ret;
}

int BaresipVidisp::alloc(struct vidisp_st **vp,
            const struct vidisp *vd, struct vidisp_prm *prm,
            const char *dev,
            vidisp_resize_h *resizeh, void *arg)
{
	(void)resizeh;
	(void)dev;
	(void)prm;
	(void)arg;

	_vidisp_st* st = (_vidisp_st*) mem_zalloc(sizeof(_vidisp_st), destroy);
	*vp = (vidisp_st*)st;

	st->vd = vd;
	st->vidisp = new BaresipVidisp();
	st->vidisp->moveToThread(QCoreApplication::instance()->thread());

	emit BaresipCore::instance().newVideo(st->vidisp);
	return 0;
}

int BaresipVidisp::display(struct vidisp_st *st, const char *title,
						   const struct vidframe *bsframe)
{
	(void)title;
	//qInfo() << "display" << title << bsframe->linesize[0];
	_vidisp_st* vst = (_vidisp_st*) st;
	vst->vidisp->uploadTexture(bsframe);

	return 0;
}

void BaresipVidisp::destroy(void* arg) {
	struct _vidisp_st* st = (_vidisp_st*) arg;
	//delete st->vidisp;
	qInfo() << "\e[35mBaresipVidisp::destroy\e[m";
	
	//st->vidisp->deleteLater();
	QMetaObject::invokeMethod(st->vidisp, "deleteLater", Qt::QueuedConnection);
}
*/
