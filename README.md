# QRLand

This project is a 3d polygonal lanscape generator based on custom build logic. Written for Linux using QT framework (uses GPU stuff from it, so it's quite not portable to Windows). No other graphic libraries are used, so it's written from scratch in all the generating and visualization stuff. 

### Short building logic description
The solution uses dynamic building with full process visualisation. So it starts with empty plane of given size (vertex size). The interface provides 3 main tools to control the generation:
- mountain tool - uses pre-defined lithospheric plates map (info about plates shapes and their movement direction & speed) to create mountains
- hill tool - simple implementation for classic hill algorithm for landscape generation
- water sources manager - rainfall and dotty water sources - which generate new water to the map

Behind the scenes, water erosion process is running to change the landscape based on water flows. Also user is provided a set of controls for the speed of generating for each tool.

Finally, you can increase the vertex-size of map to make in more detailed. The key concept is to define the basic view of the landscape on low-resolution map and then add details using same tools, but locally.

Part of interface settings example:
![image](https://user-images.githubusercontent.com/45338359/147388866-d9e021a3-20a1-4c79-aad0-62d633a42924.png)


### Concerning visualization and saving
The application provides settings for the direction of light source as well as level-of-detail (LOD) value to increase the perfomance for big maps

### Result examples
![image](https://user-images.githubusercontent.com/45338359/147388509-3ecbea83-f3ca-4f45-a3bb-f46ce14810a3.png)

![image](https://user-images.githubusercontent.com/45338359/147388515-62ffd464-044d-45a4-8544-18a86da63cd8.png)
