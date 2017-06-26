from enum import Enum

class MessageType(Enum):
    UNKNOWN_MESSAGE = 0
    REGISTER_REQUEST = 1
    REGISTER_RESPONSE = 2
    REGISTER_OBJECT_REQUEST = 3
    REGISTER_OBJECT_RESPONSE = 4
    REGISTER_SERVICE_REQUEST = 5
    REGISTER_SERVICE_RESPONSE = 6
    REGISTER_PARAMETER_REQUEST = 7
    REGISTER_PARAMETER_RESPONSE = 8
    REGISTER_OPTION_REQUEST = 9
    REGISTER_OPTION_RESPONSE = 10
    REGISTER_END_OBJECT_REQUEST = 11
    REGISTER_END_OBJECT_RESPONSE = 12

    @classmethod
    def end_messages(cls):
        return [
            cls.REGISTER_REQUEST,
            cls.REGISTER_END_OBJECT_RESPONSE
            ]
