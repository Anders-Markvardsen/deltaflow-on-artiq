// This shows a simple example of how to archive the build output artifacts.
node('linux') {
    stage("Checkout") {
        cleanWs()
        checkout scm
    }

    stage("Cleanup") {
        sh "make clean"
    }

    stage("Build images") {
        sh "make image"
    }

    stage("Build docs") {
        sh "make docs all"
        archiveArtifacts artifacts: 'docs/sphinx-build-html.log, docs/sphinx-build-pdf.log, docs/sphinx-build-epub.log'
        recordIssues(tools: [sphinxBuild(name: 'Docs',
                                         pattern: 'docs/sphinx-build-html.log',
                                         reportEncoding: 'UTF-8')])
        publishHTML([allowMissing: false,
                     alwaysLinkToLastBuild: true,
                     keepAll: false,
                     reportDir: '',
                     reportFiles: 'docs/_build/html/index.html',
                     reportName: 'Doxygen',
                     reportTitles: ''])
    }

    stage("Run deltagraph"){
        warnError('Error occured, catching exception and continuing to store test results.') {
            sh "make run-deltagraph"
        }    
    }

   stage("Run deltaruntime"){
        warnError('Error occured, catching exception and continuing to store test results.') {
            sh "make run-deltasimulator"
        }
    }

    stage("Run emulator"){
        warnError('Error occured, catching exception and continuing to store test results.') {
            sh "make run-kasli-emulator"
        }
    }

    stage("Clean container") {
        sh "make clean-container"
    }
  }

