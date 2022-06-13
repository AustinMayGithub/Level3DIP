import psycopg2

#establishing the connection
conn = psycopg2.connect(
   database="testdb", user='postgres', password='1234', host='127.0.0.1', port= '5432'
)
#Creating a cursor object using the cursor() method
cursor = conn.cursor()

#Executing an MYSQL function using the execute() method
cursor.execute("select * from items")

# Fetch a single row using fetchone() method.
print( cursor.fetchone())

#Closing the connection
conn.close()