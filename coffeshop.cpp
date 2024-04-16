#include "coffeshop.h"
#include "ui_coffeshop.h"

CoffeShop::CoffeShop(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CoffeShop)
{
    ui->setupUi(this);

    QJsonArray allProduct = this->getAllProduct();
    QVector<QString> productType = {"cold" , "hot" , "food"};
    int number = 1;
    for(int i=0 ; i<productType.size() ; i++){
        QString targetType = productType[i];
        foreach(QJsonValue value , allProduct){
            QJsonObject product = value.toObject();
            if(product["type"] == targetType){
                QFrame *newFrame = this->creatCart(product["uniqid"].toString() , product["name"].toString() , product["price"].toString() , QString::number(number++));
                ui->scrollAreaWidgetContents->layout()->addWidget(newFrame);
            }
        }
    }
    this->getAllBtn();
}

CoffeShop::~CoffeShop()
{
    delete ui;
}


void CoffeShop::on_coldType_clicked()
{
    this->showTypeOfProduct("cold");
    this->getAllBtn();
}


void CoffeShop::on_hotType_clicked()
{
    this->showTypeOfProduct("hot");
    this->getAllBtn();
}


void CoffeShop::on_foodType_clicked()
{
    this->showTypeOfProduct("food");
    this->getAllBtn();
}


void CoffeShop::on_AllProductType_clicked()
{
    this->removeAllProduct();
    QJsonArray allProduct = this->getAllProduct();
    QVector<QString> productType = {"cold" , "hot" , "food"};
    int number = 1;
    for(int i=0 ; i<productType.size() ; i++){
        QString targetType = productType[i];
        foreach(QJsonValue value , allProduct){
            QJsonObject product = value.toObject();
            if(product["type"] == targetType){
                QFrame *newFrame = this->creatCart(product["uniqid"].toString() , product["name"].toString() , product["price"].toString() , QString::number(number++));
                ui->scrollAreaWidgetContents->layout()->addWidget(newFrame);
            }
        }
    }
    this->getAllBtn();
}


void CoffeShop::on_completePurchase_clicked()
{
    this->showallMoney();
}

void CoffeShop::on_pushButton_clicked()
{
    this->removeAllProduct();
    QJsonArray jsonArray = this->getAllProduct();
    int number = 1;
    foreach(QJsonValue value, jsonArray){
        QJsonObject product = value.toObject();
        if(product["name"].toString().contains(ui->searchBar->text())){
            QFrame *newFrame = this->creatCart(product["uniqid"].toString() , product["name"].toString() , product["price"].toString() , QString::number(number++));
            ui->scrollAreaWidgetContents->layout()->addWidget(newFrame);
        }
        else{
            QString text = ui->searchBar->text();
            text [0] = text[0].toUpper();
            if(product["name"].toString().contains(text)){
                QFrame *newFrame = this->creatCart(product["uniqid"].toString() , product["name"].toString() , product["price"].toString() , QString::number(number++));
                ui->scrollAreaWidgetContents->layout()->addWidget(newFrame);
            }
        }
    }
    this->getAllBtn();
}

