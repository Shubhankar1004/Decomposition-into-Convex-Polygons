#!/usr/bin/env python
# coding: utf-8

# In[2]:


import matplotlib.pyplot as plt


# In[236]:


f = open("input4.txt","r")
y = []
for each in f:
    y.append(tuple((float(item)) for item in each.split(' ')))
y.remove(y[0])


# In[237]:


coord = y
coord.append(coord[0]) #repeat the first point to create a 'closed loop'

xs, ys = zip(*coord) #create lists of x and y values

plt.figure()
plt.plot(xs,ys) 
plt.show()


# In[238]:


f = open("output.txt","r")
x =[]
for each in f:
    x.append(tuple((int(item)) for item in each.split(' ')))


# In[239]:


for i in x:
    for j in i:
        coord.append((coord[j]))


# In[240]:


xs, ys = zip(*coord) #create lists of x and y values

plt.figure()
plt.plot(xs,ys) 
plt.show()


# In[ ]:




