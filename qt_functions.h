//  Copyright (c) 2017 Richard Games. All rights reserved.

#ifndef QT_FUNCTIONS_H_
#define QT_FUNCTIONS_H_

#include <QLayout>

namespace sk {
  inline void setVisible(QLayout *layout,
  const std::initializer_list<int> &indices, bool state) {
    for (const auto &i : indices) {
      QWidget *widget = layout->itemAt(i)->widget();
      if (widget)
        widget->setVisible(state);
    }
  }

  //! To make a object
  template <typename T>
  inline T* make(QString name, QWidget *parent = 0) {
    T *obj(new T(parent));
    obj->setObjectName(name);
    return obj;
  }

  //! To make a layout
  template <typename T>
  inline T* make(QString name, Qt::Alignment align, int spacing = 0,
  int margin = 0) {
    T *object(make<T>(name));
    object->setAlignment(align);
    object->setSpacing(spacing);
    object->setMargin(margin);
    return object;
  }

  //! To make a button with icon
  template <typename T>
  inline T* make(QString name, QString icon) {
    T *object(make<T>(name));
    object->setIcon(QIcon(icon));
    object->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    return object;
  }

}  // namespace sk

#endif  // QT_FUNCTIONS_H_
