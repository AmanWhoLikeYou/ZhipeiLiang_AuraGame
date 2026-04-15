// Zhipei Liang


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UAuraWidgetController* NewWidgetController)
{
	WidgetController = NewWidgetController;
	AfterWidgetControllerSet();
}
