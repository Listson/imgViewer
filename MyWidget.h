#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QFileDialog>
#include <QListWidget>
#include <QDir>
#include <QTimer>
class MyWidget:public QWidget{
	Q_OBJECT
	public:
		MyWidget(void);
		void setLabelImg(QString &filename);
		void setLabelPreview(int item_row);
	private:
		QString path;
		QString filepath;
		QSlider *slider;//滑动条
		QLabel *label_img;//显示图片的标签
		QPushButton *button_openfile;//打开文件按钮
		QListWidget *listWidget;
		QPushButton *button_left_arrow;//左箭头
		QPushButton *button_right_arrow;//右箭头
		QPushButton *button_auto_play;//自动播放按钮
		QVBoxLayout *left_v_layout;//左边垂直布局
		QVBoxLayout *right_v_layout;//右边垂直布局
		QHBoxLayout *right_down_h_layout;//右下水平布局
		//预览模块
		QHBoxLayout *preview_h_layout;
		QLabel *label_view[5];//预览图片的标签


		QHBoxLayout *h_layout;//总体水平布局
		QTimer *timer;
	public slots:
		void slot_openfile();
		void slot_clicked_listWidgetItem(QListWidgetItem *listItem);
		void slot_button_left_arrow();
		void slot_button_right_arrow();
		void slot_button_auto_play();
		void slot_slide_value_changed(int value);
};

#endif
