//
//  ActionResponderStatus.h
//  Othello
//
//  Created by Snow on 1/26/14.
//
//

#ifndef _ACTION_RESPONDER_STATUS_H_
#define _ACTION_RESPONDER_STATUS_H_

enum class ActionResponderStatus {
    WAITING_FOR_CONTROL_ACTION,
    NEED_TO_ASK_FOR_USER_COMFIRM,
    WAITING_FOR_USER_COMFIRM_ACTION,
    BUSY
};

#endif
