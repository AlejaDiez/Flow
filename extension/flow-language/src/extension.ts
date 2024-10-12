import * as vscode from 'vscode';
import FlowFormatter from './formatter';

export function activate(context: vscode.ExtensionContext) {
    const console = vscode.window.createOutputChannel('Flow Formatter');
    const formatter = new FlowFormatter(console);

    context.subscriptions.push(
        vscode.languages.registerDocumentFormattingEditProvider(
            'flow',
            formatter
        )
    );
}

export function deactivate() {}
