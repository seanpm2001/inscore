
///<reference path="JSSVGBase.ts"/>

class JSSVGView extends JSSvgBase {

	constructor(parent: JSObjectView) {
		super(parent);
		this.getElement().className = "inscore-svg";
	}
	clone (parent: JSObjectView) : JSObjectView { return new JSSVGView(parent); }

	toString() : string		    { return "JSSVGView"; }

	getSVGTarget() : SVGShape   { return this.fSVG; }
	updateSVGDimensions(w: number, h: number) : void { }
	updatePenControl(pen: OPen) : void {	this.updateRegularPen (pen); }

	getSvg (obj: INScoreObject) : string { return obj.getSVGInfos(); }

	updateSpecial(obj: INScoreObject, oid: number)	: boolean {
		let svg = this.getSvg (obj);
		this.fSVG.innerHTML = svg;
		let bb = this.fSVG.getBBox();
		this.updateObjectSizeSync (obj, bb.width + bb.x, bb.height + bb.y);
		return true;
    }
}
