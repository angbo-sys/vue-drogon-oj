//
// Created by xiaohua jie on 25-9-12.
// 题目模型主文件 - 统一包含所有题目相关的模型类
//

#ifndef QUESTION_MAIN_H
#define QUESTION_MAIN_H

#include "Question.h"
#include "QuestionType.h"
#include "QuestionCollection.h"

// 题目模型命名空间
namespace QuestionModels {
    // 题目类
    using Question = ::Question;
    
    // 题目类型类
    using QuestionType = ::QuestionType;
    
    // 题目集合管理类
    using QuestionCollection = ::QuestionCollection;
}

#endif //QUESTION_MAIN_H
