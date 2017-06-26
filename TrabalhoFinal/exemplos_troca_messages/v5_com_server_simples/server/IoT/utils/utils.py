import sys, struct

class Utils:
    BYTE_ORDER = ((sys.byteorder == "little") and "<" or ">")

    @staticmethod
    def pack_with_byte_order(fmt, value):
        return struct.pack((Utils.BYTE_ORDER+fmt), value)
    
    def unpack_with_byte_order(fmt, string):
        return struct.unpack((Utils.BYTE_ORDER+fmt), string)[0]

    def bytearray_array(ba):
        return ([ "0x%02x" % b for b in ba ])