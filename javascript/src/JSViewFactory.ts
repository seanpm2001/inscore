
///<reference path="inscoreGlue.ts"/>
///<reference path="JSObjectView.ts"/>
///<reference path="JSTextView.ts"/>
///<reference path="JSSceneView.ts"/>
///<reference path="JSRectView.ts"/>
///<reference path="JSEllipseView.ts"/>
///<reference path="JSLineView.ts"/>
///<reference path="JSImageView.ts"/>
///<reference path="JSArcView.ts"/>
///<reference path="JSPolygonView.ts"/>
///<reference path="JSCurveView.ts"/>
///<reference path="JSGMNView.ts"/>
///<reference path="JSXMLView.ts"/>
///<reference path="JSSVGView.ts"/>


//----------------------------------------------------------------------------
class JSViewFactory {	
	static createScene (id: string, objid: number) : number { 
		const scene = new JSSceneView(id, objid);
		return scene.getId();
	}


	static create (parentid: number, type: string, objid: number) : number	{ 
		let parent = JSObjectView.getVObject(parentid);
		let view = null;

		switch (type) {
			case "txt":
				view = new JSTextView(parent);
				view.waitForSize (objid);
				break; 			
			case "html":
				view = new JSHtmlView(parent);
				view.waitForSize (objid);
				break; 			
			
			case "rect":	 view = new JSRectView(parent); break; 
			case "ellipse":  view = new JSEllipseView(parent); break; 
			case "line": 	 view = new JSLineView(parent); break; 
			case "img":  	 view = new JSImageView(parent); break;
			case "arc": 	 view = new JSArcView(parent); break;
			case "polygon":  view = new JSPolygonView(parent); break;
			case "curve":    view = new JSCurveView(parent); break;
			case "gmn":      view = new JSGMNView(parent, gGlue.guido()); break;
			case "musicxml": view = new JSXMLView(parent, gGlue.xmllib(), gGlue.guido()); break;
			case "svg":      view = new JSSVGView(parent); break;

			case "audio":
			case "fileWatcher":
			case "graph":
			case "fastgraph":
			case "radialgraph":
			case "grid":
			case "pianoroll":
			case "pianorollstream":
			case "gmnstream":
			case "layer":
			case "memimg":
			case "rshape":
			case "sig":
			case "signode":
			case "video":
console.log ("JSViewFactory::create type " + type + " parent id: " + parent);
				break; 

			case "gmnf":
			case "pianorollf":
			case "htmlf":
			case "musicxmlf":
			case "svgf":
			case "txtf":
		console.log ("JSViewFactory::create type " + type + " parent id: " + parent);
				break; 
		
			default:
console.log ("JSViewFactory::create unknown type " + type + " parent id: " + parent);
		}
		return view ? view.getId() : 0;
	}
}

