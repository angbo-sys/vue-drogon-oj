//
// Created by xiaohua jie on 25-9-12.
// 用户模型主文件 - 统一包含所有用户相关的模型类
//

#ifndef USER_MAIN_H
#define USER_MAIN_H

#include "UserInfo/Info.h"
#include "QuestionDowm/UserQuestionDowm.h"

// 用户模型命名空间
namespace UserModels {
    // 用户信息类
    using UserInfo = ::UserInfo;
    
    // 用户问题进度类
    using UserQuestionDown = ::user_question_down;
    using QuestionFinishInfo = ::question_finish_info_struct;
}

#endif //USER_MAIN_H
