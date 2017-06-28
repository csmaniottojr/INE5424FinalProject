# encoding: utf-8
from utils.utils import Utils
from .command_message_type import CommandMessageType

"""
    Classe responsável pela serialização e 
    deserialização das mensagens de comando.
"""
class CommandSerialization(object):
    
    # Formato das mensagens de Command:
    # Byte 0            1          2       6          7        9        x   
    #      +------------+----------+-------+----------+--------+--- ~ --+
    #      | START_CHAR | msg size | SO id | msg type | reg id |  data  |
    #      +------------+----------+-------+----------+--------+--- ~ --+

    START_CHAR = ':'
    BASE_SIZE = 9

    @staticmethod
    def is_valid_message(message, length):
        return chr(message[0]) == CommandSerialization.START_CHAR and \
            CommandSerialization.deserialize_size(message) == length

    @staticmethod
    def deserialize_size(barray):
        return int(barray[1])
    
    @staticmethod
    def deserialize_emote_id(barray):
        return Utils.unpack_with_byte_order("I", barray[2:6])
    
    @staticmethod
    def deserialize_message_type(barray):
        return CommandMessageType(int(barray[6]))
    
    @staticmethod
    def deserialize_register_id(barray):
        return Utils.unpack_with_byte_order("H", barray[7:9])
    
    def deserialize_data(barray):
        size = CommandSerialization.deserialize_size(barray)
        length = size - CommandSerialization.BASE_SIZE
        if length > 0:
            return barray[9:(9+length)]
        else:
            return bytearray()
    
    @staticmethod
    def serialize(params):
        # Espera os seguintes dados:
        #   params['emote_id']  [int]
        #   params['msg_type']  [int ou CommandMessageType]
        #   params['reg_id']    [unsigned short]
        #   params['data']      [bytearray]
        result = bytearray()

        if 'data' not in params or params['data'] is None:
            params['data'] = bytearray()

        if not isinstance(params['data'], bytearray):
            return result

        result.append(ord(CommandSerialization.START_CHAR))

        result.append(CommandSerialization.BASE_SIZE + len(params['data']))

        id = Utils.pack_with_byte_order("I", params['emote_id'])
        result.extend(id)

        type = params['msg_type']
        if not isinstance(type, CommandMessageType):
            type = CommandMessageType( int(type) )
        tmp = Utils.pack_with_byte_order("B", type.value)
        result.extend(tmp)

        regId = Utils.pack_with_byte_order("H", params['reg_id'])
        result.extend(regId)

        if len(params['data']) > 0:
            result.extend(params['data'])
        
        return result