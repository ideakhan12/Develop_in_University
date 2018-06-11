from tinydb import TinyDB, Query

filepath = 'test-tinydb.json'
db = TinyDB(filepath)

# 기존에 'fruits' 라는 테이블 존재한다면 삭제
db.purge_table('fruits')
# 테이블 생성
table = db.table('fruits')

table.insert({'name':'Banana', 'price':6000})
table.insert({'name':'Orange', 'price':12000})
table.insert({'name':'Mango', 'price':8400})

print(table.all())

Item = Query()
res = table.search(Item.name == 'Orange')
print('Orange is ', res[0]['price'])

print('8000원 이상인 것 : ')
res = table.search(Item.price>=8000)

for it in res :
    print('-', it['name'])