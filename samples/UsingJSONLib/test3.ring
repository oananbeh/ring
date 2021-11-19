load "jsonlib.ring"

cJSON = `{
  "firstName": "John",
  "lastName": "Smith", 
  "age": 20,
  "address": {
    "streetAddress": "21 2nd Street",
    "city": "New York",
    "state": "NY",
    "postalCode": "10021"
  },
  "phoneNumbers": [
    { "type": "home", "number": "212 555-1234" },
    { "type": "fax", "number": "646 555-4567" }
  ]
}`

func main
	aList = Json2List(cJSON)
	? List2JSON(aList)