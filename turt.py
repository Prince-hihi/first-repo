import turtle
turtle.bgcolor("black")
t=turtle.Turtle()
colours=['red','green','blue',]
for i in range (170):
    t.forward(5*i)
    t.right(170)
    t.pencolor(colours[i%3])
    t.speed(500)
turtle.done()