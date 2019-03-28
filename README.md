# About

Based on https://habr.com/ru/post/134802/ inspired by https://doc.qt.io/qt-5/qtnetwork-torrent-example.html

Why?
* How to display a scrollable list with a substantial amount of widgets as items in a Qt C++ app?
  See
  https://stackoverflow.com/questions/45389540/how-to-display-a-scrollable-list-with-a-substantial-amount-of-widgets-as-items-i

Every widget you create has to go to the main event loop in Qt, which will make your program slow
-> No widgets, paint controls

Extra links:
* Drawing a widget in QItemDelegate's paint method
  See
  https://stackanswers.net/questions/qwidget-as-view-item-qt-model-view-controller
  https://www.qtcentre.org/Basically create a delegate inheriting QAbstractItemDelegate, with a reference to a widget which "looks" and "acts" like you want your nodes to do.
  Override editorEvent in your delegate and pass the event to your widget, remembering to set it's data first and to adjust mouse position for the widget.
  Then just use the sizeHint event to set data ( it will be called first every time your itemview needs something from your node) and render the widget to the painter in the paint event.
  Presto, virtual itemview having only one "real" widget no matter how many nodes. Tested with 50k items, works beautifully. threads/8660-Drawing-a-widget-in-QItemDelegate-s-paint-method
  https://stackoverflow.com/questions/948444/qlistview-qlistwidget-with-custom-items-and-custom-item-widgets
  NEVER USE ABSOLUTE VALUES FOR THE POSITIONS. You will never get it right, especially for different DPIs. https://stackoverflow.com/questions/44344652/paint-widget-directly-on-qlistview-with-qstyleditemdelegatepaint/45386801#45386801
  https://stackoverflow.com/questions/6505627/how-to-make-qcombobox-as-multiselect-in-qt
* QT
  https://doc.qt.io/archives/qt-5.10/qtwidgets-itemviews-stardelegate-example.html
  https://doc.qt.io/qt-5/qtwidgets-itemviews-spinboxdelegate-example.html
* GitHub
  https://github.com/scopchanov/SO-CustomList/tree/master/app/src

Also see:
* >>> QDataWidgetMapper <<<
  books
  examples
  https://github.com/semiguy/qt5-sample/tree/e128b3c3b3df28b5c9290655e5582e66ff219a82/QDataWidgetMapper http://qt.shoutwiki.com/wiki/Mapping_of_StandardItemModel_via_DataWidgetMapper_in_Qt
  https://github.com/savolai/qt_qradiobutton_qdatawidgetmapper_example
  https://evileg.com/ru/tag/QDataWidgetMapper%20example/
  http://www.crossplatform.ru/node/314
  https://felgo.com/doc/qt/qtwidgets-itemviews-combowidgetmapper-example/
  https://felgo.com/doc/qt/qtsql-sqlwidgetmapper-example/
  Qt Training: Model/View II (part 3/5): Data Widget Mapper: Oliver ... https://www.youtube.com/watch?v=GFdW0VdJqw8
* DataWidgetBinder
  https://github.com/Winded/VPKViewer/blob/09604b6d5e0c634c8decf2d52071340e71894a5f/src/configdialog.cpp
* https://github.com/pawelsalawa/sqlitestudio/wiki/Model-view_binding
* Dummy Delegate
  https://stackoverflow.com/a/29164717
* https://habr.com/ru/post/276419/ Гибридный класс, содержащий в себе стандартную фабрику и переопределяющий createEditor для пользовательских типов.
* Widget::grab()
  https://stackoverflow.com/a/43109044
  https://www.cnblogs.com/foohack/p/8108446.html
* https://felgo.com/doc/qt/model-view-programming/
  https://felgo.com/apps/avoid-cpp-models-qt
* use a QStringList as a backend for a custom model.
  https://doc.qt.io/archives/qt-4.8/qt4-interview.html#creating-a-custom-model (see "Creating a Custom Model)
* Udemy
  Mastering Qt 5 GUI Programming | Udemy
  https://www.udemy.com/mastering-qt-5-gui-programming/
  Create advanced, custom model views and web views.
* Создание моделей данных для QComboBox https://habr.com/ru/post/329228/
* GRPC https://habr.com/ru/post/420237/
* QML Model
  https://habr.com/ru/post/349822/
  Efficient models for QML with QSyncable
  https://habr.com/ru/post/302428/
* TODO
  Plot https://habr.com/ru/post/266957/
  Функциональное тестирование программ на Qt https://habr.com/ru/post/301702/
  https://habr.com/ru/post/354442/
  https://habr.com/ru/post/321426/ QQuickPaintedItem
* https://habr.com/ru/post/247207/ При создании экземпляра класса DynamicTranslator мы указываем целевой объект, перевод, а также имя слота (например, setWindowTitle) или имя свойства (windowTitle). Наш DynamicTranslator при каждой смене языка либо вызывает соответствующий слот при помощи QMetaObject, либо устанавливает нужное свойство при помощи setProperty.
* QML opengl
  https://habr.com/ru/post/303722/
  https://habr.com/ru/post/248103/
* QT Server https://habr.com/ru/post/263549/
* MVP https://habr.com/ru/post/246445/
