from sqlalchemy import create_engine
from src.models import *
from sqlalchemy.orm import sessionmaker
engine = create_engine('mysql+pymysql://root:root@localhost/sodb_novo')
Session = sessionmaker(bind=engine)
session = Session()

smart_obj = session.query(SmartObject).filter(
    SmartObject.device_id == 450).first()
print smart_obj

smart_obj = session.query(SmartObject).filter(
    SmartObject.device_id == 451).first()
print smart_obj
# so = SmartObject('meu objeto fofinho', 450)
#
# s1 = Service('servico1')
# s1.add_parameter(ParameterBoolean('booleano'))
# s1.add_parameter(ParameterInteger('inteiro',2,5))
# s2 = Service('servico2')
# s2.add_parameter(ParameterFloat('pto flutuante', 3.4, 10.6))
#
# p_op = ParameterOption('p options')
# p_op.add_option(Option('op1'))
# p_op.add_option(Option('op2'))
# s2.add_parameter(p_op)
# so.add_service(s1)
# so.add_service(s2)
#
# session.add(so)
# session.commit()
#
# print so
