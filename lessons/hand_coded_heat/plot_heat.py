import sys, time

hostName = 'scratlantis'

hp0=GetMachineProfile(hostName)
hp1=GetMachineProfile(hostName)
hp1.ClearLaunchProfiles()
hp1.AddLaunchProfiles(hp0.GetLaunchProfiles(0))
OpenComputeEngine(hp1)

ca = CurveAttributes()
ca.lineWidth = 1
ca.designator = ""
ca.showLegend = 0
ca.showLabels = 0
ca.curveColor = (0, 0, 255, 255)
#ca.showPoints = 1
ca.symbol = ca.Circle
ca.pointSize = 5

#SetWindowLayout(2)
SetActiveWindow(1)
OpenDatabase("heat_soln_*.curve database",0)
AddPlot("Curve","curve")
SetPlotOptions(ca)
DrawPlots()
v = GetViewCurve()
v.viewportCoords = (0.2, 0.95, 0.15, 0.85)
SetViewCurve(v)
if v.rangeCoords[1] - v.rangeCoords[0] < 2:
    v.domainCoords = (-0.1, 1.1)
    v.rangeCoords = (-0.1, 1.1)
    SetViewCurve(v)

#SetActiveWindow(2)
#DeleteAllPlots();
#val = OpenDatabase("error.curve")
#if val:
#    AddPlot("Curve","curve")
#    ca.designator = "Error"
#    ca.curveColor = (255, 0, 0, 255)
#    ca.showPoints = 0
#    SetPlotOptions(ca)
#OpenDatabase("residual.curve")
#AddPlot("Curve","curve")
#DrawPlots()

#SetActiveWindow(1)
for i in range(TimeSliderGetNStates()-1):
    time.sleep(0.1)
    TimeSliderNextState()
    ResetView()
time.sleep(10)
sys.exit(0)
