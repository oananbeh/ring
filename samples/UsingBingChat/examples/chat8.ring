#================================================#
# Author: Bing Chat 
# Questions by Fayed
# Note: It's not the perfect code (It just works!)
#================================================#

# Create a list of objects from different classes that implement a common interface (the Draw method)
shapes = [new Circle, new Square, new Triangle]

# Loop through the list and call the Draw method for each object using dynamic binding (polymorphism)
for shape in shapes 
    shape.Draw()
next

# Define an abstract class Shape that has an abstract method Draw 
class Shape 

    # Declare an abstract method Draw 
    func Draw 
        return self 
    end 

end 

# Define a Circle class that inherits from Shape and implements Draw 
class Circle < Shape 

    # Implement Draw 
    func Draw 
        see "Drawing a circle!" + nl
        return self 
    end 

end 

# Define a Square class that inherits from Shape and implements Draw 
class Square < Shape 

    # Implement Draw 
    func Draw 
        see "Drawing a square!" + nl
        return self 
    end 

end 

# Define a Triangle class that inherits from Shape and implements Draw 
class Triangle < Shape 

   # Implement Draw  
   func Draw  
       see "Drawing a triangle!" + nl
       return self  
   end  

end  