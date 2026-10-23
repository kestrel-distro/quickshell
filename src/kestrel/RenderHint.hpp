#pragma once

#include <qcoreapplication.h>
#include <qevent.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qquickwindow.h>
#include <unistd.h>

#include "KestrelHint.hpp"

namespace KestrelRenderHint {

// Qt's scene graph render thread is created per window when the window is first
// shown and lives outside the process' startup thread set, so setProcess() at
// startup cannot reach it. beforeRendering() is documented as emitted on the
// scene graph rendering thread; connect directly and hint that thread once.
class Filter: public QObject {
public:
	using QObject::QObject;

	bool eventFilter(QObject* obj, QEvent* event) override {
		if (event->type() == QEvent::Show) {
			auto* window = qobject_cast<QQuickWindow*>(obj);
			if (window != nullptr && !window->property("_kestrel_render_hinted").toBool()) {
				window->setProperty("_kestrel_render_hinted", true);
				QObject::connect(
				    window,
				    &QQuickWindow::beforeRendering,
				    window,
				    [] {
					    static thread_local bool hinted = false;
					    if (!hinted) {
						    hinted = true;
						    KestrelHint::setTid(static_cast<pid_t>(::gettid()), 1);
					    }
				    },
				    Qt::DirectConnection
				);
			}
		}

		return QObject::eventFilter(obj, event);
	}
};

inline void install(QCoreApplication* app) {
	if (app != nullptr) app->installEventFilter(new Filter(app));
}

} // namespace KestrelRenderHint
