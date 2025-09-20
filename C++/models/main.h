//
// Created by xiaohua jie on 25-9-12.
// 模型主文件 - 统一包含所有模型类
//

#ifndef MODELS_MAIN_H
#define MODELS_MAIN_H

// 用户模型
#include "User/main.h"

// 题目模型
#include "Question/main.h"

// 全局模型命名空间
namespace Models {
    // 用户相关模型
    namespace User {
        using UserInfo = UserModels::UserInfo;
        using UserQuestionDown = UserModels::UserQuestionDown;
        using QuestionFinishInfo = UserModels::QuestionFinishInfo;
    }
    
    // 题目相关模型
    namespace Question {
        using Question = QuestionModels::Question;
        using QuestionType = QuestionModels::QuestionType;
        using QuestionCollection = QuestionModels::QuestionCollection;
    }
}

#endif //MODELS_MAIN_H
