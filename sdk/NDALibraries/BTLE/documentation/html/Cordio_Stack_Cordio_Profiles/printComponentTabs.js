var arr     = []
var strgURL = location.pathname                   // Path of current component

// Constuctor for the array of objects
function tabElement(id, value, folderName, tabTxt) {
  this.id              = id                     // ElementID as needed in html;
  this.folderName      = folderName             // Folder name of the component
  this.tabTxt          = tabTxt                 // Text displayed as menu on the web
  this.value           = value                  // Order of the tab
  this.currentListItem = '<li id="' + this.id + '" value="' + this.value + '" class="current"> <a href="../' + this.folderName + 'index.html"><span>' + this.tabTxt + '</span></a></li>'
  this.listItem        = '<li id="' + this.id + '" value="' + this.value + '">' +            ' <a href="../' + this.folderName + 'index.html"><span>' + this.tabTxt + '</span></a></li>'
}

// Write tabs: Called from the header file.
function writeComponentTabs() {
  var imageBCSS = new Image()
  imageBCSS.src = "../BLE_Controller/Arm_Cordio.png"
  imageBCSS.onload = function(){
    arr[1] = new tabElement("BC_SS", "1", "BLE_Controller/", "Cordio Link: BLE Controller Subsystem")
  }
  imageBCSS.onerror = function(){
    return true
  }
  var imageBB = new Image()
  imageBB.src = "../Baseband_API/Arm_Cordio.png"
  imageBB.onload = function() {
    arr[2] = new tabElement("BB_API", "2",  "Baseband_API/",    "Baseband API")
  }
  imageBB.onerror = function() {
    return true
  }
  var imageLL = new Image()
  imageLL.src = "../Link_Layer_API/Arm_Cordio.png"
  imageLL.onload = function() {
    arr[3] = new tabElement("LL_API", "3",  "Link_Layer_API/",  "Link Layer API")
  }
  imageLL.onerror = function() {
    return true
  }
  var imageLT = new Image()
  imageLT.src = "../Link_Tester_API/Arm_Cordio.png"
  imageLT.onload = function() {
    arr[4] = new tabElement("LL_TEST", "4", "Link_Tester_API/", "Link Layer Tester API")
  }
  imageLT.onerror = function() {
    return true
  }
  var imageBSP = new Image()
  imageBSP.src = "../BSP_API/Arm_Cordio.png"
  imageBSP.onload = function() {
    arr[5] = new tabElement("BSP_API", "5", "BSP_API/", "BSP API")
  }
  imageBSP.onerror = function() {
    return true
  }
  var imageCSP = new Image()
  imageCSP.src = "../CSP_API/Arm_Cordio.png"
  imageCSP.onload = function() {
    arr[6] = new tabElement("CSP_API", "6", "CSP_API/", "CSP API")
  }
  imageCSP.onerror = function() {
    return true
  }
  var imageStack = new Image()
  imageStack.src = "../Cordio_Stack_Cordio_Profiles/Arm_Cordio.png"
  imageStack.onload = function() {
    arr[7] = new tabElement("CORDIO_STACK_PROFILES", "7", "Cordio_Stack_Cordio_Profiles/", "Cordio Stack and Cordio Profiles")
  }
  imageStack.onerror = function() {
    return true
  }
  var imageWsf = new Image()
  imageWsf.src = "../WSF/Arm_Cordio.png"
  imageWsf.onload = function() {
    arr[8] = new tabElement("WSF", "8", "WSF/", "WSF")
  }
  imageWsf.onerror = function() {
    return true
  }

  window.onload = function () {
    for ( var i=1; i < arr.length; i++ ) {
      if ( arr[i] != undefined ) {
        if (strgURL.search(arr[i].folderName) > 0) {                             // If this is the current folder
          document.getElementById('Tabs').innerHTML += arr[i].currentListItem;   // Print and hightlight the tab
        } else {
          document.getElementById('Tabs').innerHTML += arr[i].listItem;          // Just print the tab
        }
      }
    }
  }
}

