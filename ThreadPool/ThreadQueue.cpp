#include"ThreadQueue.h"
ThreadQueue::ThreadQueue() {
    mSize = 0;
    mHead = new ThreadNode();
    mTail = new ThreadNode();
    mHead->mNext = mTail;
    mHead->mBefore = mTail;
    mTail->mNext = mHead;
    mTail->mBefore = mHead;
}

ThreadQueue::~ThreadQueue() {
    deleteAllThreadNode();
    if (mHead != NULL) {
        delete mHead;
        mHead = NULL;
    }
    if (mTail != NULL) {
        delete mTail;
        mTail = NULL;
    }
}

ThreadNode *ThreadQueue::addThreadNodeToHead(ThreadNode *tn) {
    tn->mNext = mHead->mNext;
    mHead->mNext->mBefore = tn;
    tn->mBefore = mHead;
    mHead->mNext = tn;
    mSize = mSize + 1;
    return tn;
}

ThreadNode *ThreadQueue::deleteThreadNodeFromTail() {
    if (mSize != 0) {
        ThreadNode *tempNode = mTail->mBefore;
        mTail->mBefore = tempNode->mBefore;
        tempNode->mBefore->mNext = mTail;
        mSize = mSize - 1;
        return tempNode;
    } else {
        printf("deleteThreadNodeFromTail:ThreadQueue is empty\n");
        return NULL;
    }
}

ThreadNode *ThreadQueue::deleteThreadNode(ThreadNode *tn) {
    if (!isEmpty()) {
        if(findThreadNode(tn) != NULL) {
            ThreadNode *before = tn->mBefore;
            ThreadNode *after = tn->mNext;
            before->mNext = after;
            after->mBefore = before;
            mSize = mSize - 1;
            return tn;
        } else {
            printf("deleteThreadNode:Thread node not in ThreadQueue\n");
            return NULL;
        }
    } else {
        printf("deleteThreadNode:ThreadQueue is empty\n");
        return NULL;
    }
}

ThreadNode *ThreadQueue::findThreadNode(ThreadNode *tn) {
    for(ThreadNode *index = mHead; index != NULL; index = index->mNext) {
        if (index == tn) {
            return index;
        }
    }
    return NULL;
}


void ThreadQueue::deleteAllThreadNode() {
    if (!isEmpty()) {
        ThreadNode *index = NULL;
        for(index = mHead->mNext; index != mTail; index = mHead->mNext) {
            mHead->mNext = index->mNext;
            index->mNext->mBefore = mHead;
            if (index != NULL) {
                delete index;
            }
            mSize = mSize - 1;
        }
    }
}
