#pragma once

#include <QDialog>
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>

class WinnerDialog : public QDialog {
	Q_OBJECT

public:
	WinnerDialog(const QString& text, QWidget * parent = 0)
		: QDialog(parent)
	{
		QVBoxLayout *mainLayout = new QVBoxLayout;
		this->setModal(true);
		this->setWindowTitle("Connect-Four Who won?");
		this->resize(260, 180);
		this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
		this->setLayout(mainLayout);


		QFont sansFont("Helvetica [Cronyx]", 18); 
		QFont f("Arial", 10, QFont::Bold);
		QLabel *label = new QLabel();
		label->setStyleSheet(
			"font: large 28px; font-weight: bold; text-align: center; color: rgb(200,100,150)"
		);
		label->setText(text);
		mainLayout->addWidget(label);
	}
};