#include "MyWidget.h"
int t=1;
MyWidget::MyWidget(void){
	path.append("/");
	left_v_layout = new QVBoxLayout;//左边的垂直布局
	button_openfile = new QPushButton("文件");//文件按钮
	listWidget = new QListWidget;//列表框
	listWidget->setMaximumWidth(150);
	listWidget->setMinimumWidth(150);
	left_v_layout->addWidget(button_openfile);
	left_v_layout->addWidget(listWidget);

	right_v_layout = new QVBoxLayout;
	slider = new QSlider(Qt::Horizontal);
	slider->setDisabled(true);//刚开始禁用
	label_img = new QLabel;
	label_img->setScaledContents(true);//图片自适应
	label_img->setMaximumSize(1500,1500);
	label_img->setMinimumSize(600,400);
	//label_img->setFixedSize(500,500);
	//预览模块
	preview_h_layout = new QHBoxLayout;
	for(int i=0; i<5; i++){
		label_view[i] = new QLabel;
		//label_img->setFixedSize(500,500);
		label_view[i]->setScaledContents(true);//图片自适应
		label_view[i]->setMaximumSize(250,150);
		label_view[i]->setMinimumSize(50,50);
		preview_h_layout->addWidget(label_view[i]);
	}

	right_v_layout->addWidget(slider);
	right_v_layout->addWidget(label_img);
	right_v_layout->addLayout(preview_h_layout);

	right_down_h_layout = new QHBoxLayout;
	button_left_arrow = new QPushButton("上一张");
	button_left_arrow->setDisabled(true);
	button_auto_play = new QPushButton("自动播放");
	button_auto_play->setDisabled(true);
	button_right_arrow = new QPushButton("下一张");
	button_right_arrow->setDisabled(true);
	right_down_h_layout->addWidget(button_left_arrow);
	right_down_h_layout->addWidget(button_auto_play);
	right_down_h_layout->addWidget(button_right_arrow);

	right_v_layout->addLayout(right_down_h_layout);

	h_layout = new QHBoxLayout;
	h_layout->addLayout(left_v_layout);
	h_layout->addLayout(right_v_layout);
	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_button_right_arrow()));

	connect(button_openfile, SIGNAL(clicked()), this, SLOT(slot_openfile()));
	connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slot_clicked_listWidgetItem(QListWidgetItem*)));
	connect(button_left_arrow, SIGNAL(clicked()), this, SLOT(slot_button_left_arrow()));
	connect(button_right_arrow, SIGNAL(clicked()), this, SLOT(slot_button_right_arrow()));
	connect(button_auto_play, SIGNAL(clicked()), this, SLOT(slot_button_auto_play()));
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slot_slide_value_changed(int)));
	setLayout(h_layout);
}
void MyWidget::slot_openfile(){
	listWidget->clear();
	path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path,QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
	QDir *dir=new QDir(path);

	QStringList filter;
	filter<<"*.png"<<"*.jpg";
	dir->setNameFilters(filter);
	QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
	//清除之前的列表项
	//文件数目:fileInfo->count();
	//文件名称:fileInfo->at(i).fileName();
	//文件路径(包含文件名):fileInfo->at(i).filePath();
	//printf("running:%d\n", __LINE__);
	for(int i=0; i<fileInfo->count(); i++){
		listWidget->addItem(fileInfo->at(i).fileName());
	}
	if(fileInfo->count()>0){
		button_left_arrow->setEnabled(true);
		button_auto_play->setEnabled(true);
		button_right_arrow->setEnabled(true);
		slider->setEnabled(true);
		//设置滑块的范围
		slider->setRange(0, fileInfo->count()-1);
		//清楚label中的图片
		//label_img->setPixmap();
	}
	else{
		button_left_arrow->setDisabled(true);
		button_auto_play->setDisabled(true);
		button_right_arrow->setDisabled(true);
		slider->setDisabled(true);
	}
}
void MyWidget::slot_clicked_listWidgetItem(QListWidgetItem *listWidgetItem){
	QString filename = listWidgetItem->text();
	//将Qstring转换为字符串
	//QByteArray ba(str.toAscii());//toAscii()返回一个QByteArray类型
	//printf("%s\n", ba.data());//data()返回char *
	//设置大图
	setLabelImg(filename);
	//获取当前的行
	int item_row = listWidget->row(listWidgetItem);
	//设置滑块的值
	slider->setValue(item_row);
	//预览
	setLabelPreview(item_row);
}
void MyWidget::setLabelImg(QString &filename){
	filepath.clear();
	filepath.append(path).append("/").append(filename);//拼接文件的绝对路径
	QByteArray ba(filepath.toAscii());//toAscii()返回一个QByteArray类型
	printf("%s\n", ba.data());//data()返回char *
	QPixmap img(filepath);
	label_img->setPixmap(img);
	label_img->resize(img.width(),img.height());
}
void MyWidget::setLabelPreview(int item_row){
	QListWidgetItem *listWidgetItem;
	QString preview_filepath;
	int row;
	for(int i=-2; i<3; i++){
		row = i+item_row;
		if(row<0)
			row = i+listWidget->count();
		if(row>listWidget->count()-1)
			row = i-1;
		//printf("%d,%d,%d\n", i, item_row,  row);
		//获取当前行的列表项信息
		listWidgetItem = listWidget->item(row);
		QString filename = listWidgetItem->text();
		preview_filepath.clear();
		preview_filepath.append(path).append("/").append(filename);//拼接文件的绝对路径
		//QByteArray ba(preview_filepath.toAscii());//toAscii()返回一个QByteArray类型
		//printf("%s\n", ba.data());//data()返回char *
		QPixmap img(preview_filepath);
		label_view[i+2]->setPixmap(img);
	}
}
void MyWidget::slot_button_left_arrow(){
	//获取当前的行
	int current_row = listWidget->currentRow();
	//如果到达第一张图片,则继续显示最后一张图片
	if(current_row==0)
		current_row = listWidget->count();
	//获得前一张图片
	QListWidgetItem *listWidgetItem = listWidget->item(current_row-1);
	//修改当前的行数为前一张图片
	listWidget->setCurrentRow(current_row-1);
	QString filename = listWidgetItem->text();
	setLabelImg(filename);
	//设置滑块的值
	slider->setValue(current_row-1);
	//预览
	setLabelPreview(current_row-1);
}
void MyWidget::slot_button_right_arrow(){
	//获取当前的行
	int current_row = listWidget->currentRow();
	//如果到达第一张图片,则继续显示最后一张图片
	if(current_row==listWidget->count()-1)
		current_row = -1;
	//获得前一张图片
	QListWidgetItem *listWidgetItem = listWidget->item(current_row+1);
	//修改当前的行数为前一张图片
	listWidget->setCurrentRow(current_row+1);
	QString filename = listWidgetItem->text();
	setLabelImg(filename);
	//设置滑块的值
	slider->setValue(current_row+1);
	//预览
	setLabelPreview(current_row+1);
}
void MyWidget::slot_button_auto_play(){
	if(t){
		button_auto_play->setText("暂停播放");
		timer->start(1000);
		t=0;
	}
	else{
		button_auto_play->setText("自动播放");
		timer->stop();
		t=1;
	}
}
void MyWidget::slot_slide_value_changed(int value){
	//获取当前行的列表项信息
	QListWidgetItem *listWidgetItem = listWidget->item(value);
	//修改当前的行数
	listWidget->setCurrentRow(value);
	QString filename = listWidgetItem->text();
	setLabelImg(filename);
	//预览
	setLabelPreview(value);
}

