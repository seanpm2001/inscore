
///<reference path="JSSVGView.ts"/>
///<reference path="TFileBased.ts"/>

class JSSVGfView extends JSSVGView 
{
	fContent = new TFileBased;
    
    toString() : string			            { return "JSSVGfView"; }
	getSvg (obj: INScoreObject) : string    { 
        let svg = this.fContent.get();
        return svg ? svg : ""; 
    }

    updateSpecial ( obj: INScoreObject, oid: number)	: boolean {		
        let address = obj.getOSCAddress();
        let inscore = gGlue.inscore();
        let pending = (): void => { inscore.postMessageStr (address, "refresh"); };
		if (this.fContent.getData (obj.getFile(), this.getElement(), pending)) 
			return super.updateSpecial (obj, oid);
		return false;
    }
}