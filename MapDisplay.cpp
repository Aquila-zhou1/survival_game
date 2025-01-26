#include "MapDisplay.h"


void createAnimatedSprite(QGraphicsScene* scene, const QString& spriteSheetPath, int frameCount, int frameWidth, int frameHeight, int x, int y, QGraphicsPixmapItem** item) {
    // 加载精灵图（包含多个帧）
    QPixmap spriteSheet(spriteSheetPath);

    if (spriteSheet.isNull()) {
        qDebug() << "Failed to load sprite sheet from path: " << spriteSheetPath;
        return;
    }

    // 创建 QGraphicsPixmapItem 并设置初始帧
    *item = new QGraphicsPixmapItem();
    (*item)->setPos(x, y);
    scene->addItem(*item);

    int currentFrame = 0; // 当前帧
    QTimer* timer = new QTimer(); // 定时器

    QObject::connect(timer, &QTimer::timeout, [=]() mutable {  // Capture by reference and make the lambda mutable
        // 计算当前帧的位置
        int row = currentFrame / (spriteSheet.width() / frameWidth);
        int col = currentFrame % (spriteSheet.width() / frameWidth);

        // 从精灵图中裁剪当前帧的图像
        qDebug() << "开始剪裁frame";
        QPixmap frame = spriteSheet.copy(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
        qDebug() << "开始剪裁frame1";
        // 更新图形项的图像
        (*item)->setPixmap(frame);
        qDebug() << "开始剪裁frame2";
        // 更新到下一个帧
        currentFrame = (currentFrame + 1) % frameCount;
        qDebug() << "开始剪裁frame3";
    });

    // 设置定时器，每100ms切换一次帧
    timer->start(100); // 每100毫秒切换一次帧
    qDebug() << "完成函数";
}
